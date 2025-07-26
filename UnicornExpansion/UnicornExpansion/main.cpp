#pragma warning (disable: 4275)

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <filesystem>
#include <string>
#include <filesystem>
#include <map>
#include "Animation.h"

#include "version.h"

#include "Game.h"
#include "GameUnit.h"
#include "Countdown.h"
#include "MiniMap.h"
#include "Texts.h"
#include "SfmlTools.h"
#include "UnitFactory.h"
#include "ComponentUnicorn.h"
#include "ComponentMeleeEnemy.h"
#include "ComponentEnemy.h"
#include "ComponentResource.h"
#include "ComponentHarvester.h"
#include "TreeBuilder.h"

#pragma comment (lib, "sfml-graphics.lib")
#pragma comment (lib, "sfml-system.lib")
#pragma comment (lib, "sfml-window.lib")
#pragma comment (lib, "sfml-audio.lib")
#pragma comment (lib, "sfml-main-s.lib")
#pragma comment (lib, "zetscript-2-1-0-static.lib")
#pragma comment (lib, "jsoncpp_static.lib")

// Размеры камеры и скорость прокрутки камеры
const int SCROLLSPEED = 10;
const float VIEW_SIZE_X = 1024.0f;
const float VIEW_SIZE_Y = 576.0f;

// Все данные игры, спрайты, тексты, массивы территорий и цветов
std::vector <std::unique_ptr<sf::Texture>> textures;
std::map<std::string, std::unique_ptr<sf::Sprite>> spr_units;
std::map<std::string, std::unique_ptr<sf::Sprite>> spr_icons;
std::map<std::string, std::unique_ptr<sf::Sprite>> spr_actions;
std::map<Terrain, std::unique_ptr<sf::Sprite>> spr_terrains;
std::map<Terrain, sf::Color> color_terrains;
std::map<LaserType, sf::Color> color_lasers;
std::map<TreeType, std::unique_ptr<sf::Sprite>> spr_trees;
sf::View view;
sf::Countdown counter_errmsg;
MiniMap minimap;
Texts texts;
Game game;
TreeBuilder treebuilder;
// Номер выделенного юнита
std::optional<int> selected_uid;
int started_galop_uid;
sf::Countdown counter_endgame;
int tekscale;
sf::RectangleShape rect_health;

// Переключатели сцен
enum class Scene { Menu, Task, Game };
Scene scene;

// Добавить юнит в массив по последней загруженной текстуре
void addUnitSpriteFromLastTexture(const std::string& code) {
    spr_units[code] = std::make_unique<sf::Sprite>(*textures.back());
    spr_units[code]->setOrigin({ textures.back()->getSize().x / 2.0f,textures.back()->getSize().y / 2.0f });

    spr_icons[code] = std::make_unique<sf::Sprite>(*textures.back());
    spr_icons[code]->setOrigin({ textures.back()->getSize().x / 2.0f,textures.back()->getSize().y / 2.0f });
    spr_icons[code]->setScale({ 48.0f / textures.back()->getSize().x, 48.0f / textures.back()->getSize().y });
}

// Добавить юнит в массив с текстурой из файла
void addUnitSprite(const std::string& code, const std::string& filename) {
    textures.push_back(std::make_unique<sf::Texture>(filename));
    addUnitSpriteFromLastTexture(code);
}

void addTreeSprite(TreeType ttype, const std::string& filename) {
    textures.push_back(std::make_unique<sf::Texture>(filename));
    spr_trees[ttype] = std::make_unique<sf::Sprite>(*textures.back());
}

// Добавление спрайта территорий
void addTerrainSprite(Terrain code, const std::string& filename) {
    textures.push_back(std::make_unique<sf::Texture>(filename));
    spr_terrains[code] = std::make_unique<sf::Sprite>(*textures.back());
}

// Добавление спрайта действий
void addActionSprite(const std::string & code, const std::string& filename) {
    textures.push_back(std::make_unique<sf::Texture>(filename));
    spr_actions[code] = std::make_unique<sf::Sprite>(*textures.back());
}

// Обновление мини-карты
void updateMiniMap(const Game & game) {
    for (int i = 0; i < game.getWidth(); i++)
        for (int j = 0; j < game.getHeight(); j++) {
            if (game.isFog(i, j))
                minimap.setCellColor(i, j, sf::Color::Black);
            else
                if (color_terrains.count(game.getMap(i, j)) > 0)
                    minimap.setCellColor(i, j, color_terrains[game.getMap(i, j)]);
        }

    // Работаем со всеми юнитами, без деления на свои и чужие
    for (int i = 0; i < game.getUnitCount(); i++)
        for (int dx = 0; dx < game.getUnit(i).getSize().x; dx++)
            for (int dy = 0; dy < game.getUnit(i).getSize().y; dy++) {
                if (game.isFog(game.getUnit(i).getXY().x + dx, game.getUnit(i).getXY().y + dy)) continue;
                sf::Color color;
                if (game.getUnit(i).getUID() == *selected_uid) {
                    color = sf::Color::Green;
                    if (game.getUnit(i).isComponent<ComponentResource>()) color = sf::Color::Blue;
                    if (game.getUnit(i).isComponent<ComponentEnemy>()) color = sf::Color::Red;
                }
                else {
                    color = sf::Color{ 0,192,0 };
                    if (game.getUnit(i).isComponent<ComponentResource>()) color = sf::Color{ 0,0,192 };
                    if (game.getUnit(i).isComponent<ComponentEnemy>()) color = sf::Color{ 192,0,0 };
                }
                minimap.setCellColor(game.getUnit(i).getXY().x + dx, game.getUnit(i).getXY().y + dy, color);
            }

    minimap.setCenter(view.getCenter().x, view.getCenter().y);
}

// Градиент цвета по остатку здоровья
sf::Color getColorByHPNorm(float hp_norm) {
    if (hp_norm < 0.5f) return sf::Color::Red;
    if (hp_norm < 0.75f) return sf::Color(255,255,0);
    return sf::Color::Green;
}

sf::Vector2f getActionButtonPos(int i) {
    const int ACTION_COL_COUNT = 5;
    return { (float)(1024 - 400 + 8 + (i % ACTION_COL_COUNT) * 76), (float)(768 - 192 + 10 + (i / ACTION_COL_COUNT) * 76) };
}

// Рисование лазера и его анимации в точке приложения
void drawLaserFromTo(sf::RenderWindow & window, sf::RectangleShape & laser, const Laser & laz, Animation & laser_anim) {
    laser_anim.setColor(color_lasers[laz.type]);
    laser_anim.setPosition(laz.pos2);
    window.draw(laser_anim);

    if ((laser_anim.getFrame() > 1) && (laser_anim.getFrame() < 14)) {
        sf::Vector2f dir = laz.pos2 - laz.pos1;
        laser.setFillColor(color_lasers[laz.type]);
        laser.setSize({ dir.length(), 3 });
        laser.setPosition(laz.pos1);
        laser.setRotation(dir.angle());
        window.draw(laser);
    }
}

void fixCameraPosition() {
    if (view.getCenter().x < view.getSize().x / 2.0f) view.setCenter({ view.getSize().x / 2.0f, view.getCenter().y });
    if (view.getCenter().x > game.getWidth() * BLOCKW - view.getSize().x / 2.0f) view.setCenter({ game.getWidth() * BLOCKW - view.getSize().x / 2.0f, view.getCenter().y });
    if (view.getCenter().y < view.getSize().y / 2.0f) view.setCenter({ view.getCenter().x, view.getSize().y / 2.0f });
    if (view.getCenter().y > game.getHeight() * BLOCKH - view.getSize().y / 2.0f) view.setCenter({ view.getCenter().x, game.getHeight() * BLOCKH - view.getSize().y / 2.0f });
}

void updateScale() {
    view.setSize({ VIEW_SIZE_X * (0.5f + tekscale * 0.25f), VIEW_SIZE_Y * (0.5f + tekscale * 0.25f) });
    minimap.setWindowSize(view.getSize().x, view.getSize().y);
    fixCameraPosition();
}

void drawHealthRectAt(sf::RenderWindow& window, float hperc, float basew, float x, float y) {
    rect_health.setFillColor(getColorByHPNorm(hperc));
    int cntrect = ((int)(basew * hperc) / (rect_health.getSize().x + 2)) + 1;
    for (int j = 0; j < cntrect; j++) {
        rect_health.setPosition({ x + j * (rect_health.getSize().x + 2), y });
        window.draw(rect_health);
    }
}

// Загрузчик игры из файлов
void loadGame(int leveln) {
    bool paramok = game.loadConfigs();
    bool mapok = game.loadMap("levels\\level" + std::to_string(leveln) + ".map");
    bool scriptok = game.loadScript("levels\\level" + std::to_string(leveln) + ".script");
    
    game.update(0.0); // Первичная инициализация для тумана войны
    tekscale = 2.0;
    updateScale();
    view.setCenter({ (float)game.getInitialView().x * BLOCKW, (float)game.getInitialView().y * BLOCKH });

    selected_uid = std::nullopt;

    minimap.prepareMiniMap(8, 768 - 192 + 8, 192 - 16, 192 - 16, game.getWidth(), game.getHeight(),
        VIEW_SIZE_X, VIEW_SIZE_Y, game.getWidth() * BLOCKW, game.getHeight() * BLOCKH);
    updateMiniMap(game);

    treebuilder.updateByGame(game);
}

int main(int argc, char * argv[])
{
    std::string exedir = ".";
    const size_t last_slash_idx = std::string(argv[0]).rfind('\\');
    if (std::string::npos != last_slash_idx) exedir = std::string(argv[0]).substr(0, last_slash_idx);
    
    // Установка текущего каталога
    if (argc > 1)
        std::filesystem::current_path(std::string(argv[1]));
    else
        std::filesystem::current_path(exedir);
    
    texts.loadFromFile("strings.txt");

    // Создание окна
    sf::RenderWindow window(sf::VideoMode({ 1024, 768 }), texts.getSfmlStr("Text_GameCaption"),sf::Style::Close);
    window.setMouseCursorVisible(false);
    window.setVerticalSyncEnabled(true);
    window.setIcon(sf::Image("images\\icon.png"));

    // Загрузка всех ресурсов
    const sf::Texture texture0("images\\intro.png");
    sf::Sprite spr_intro(texture0);

    const sf::Texture texture1("images\\border.png");
    sf::Sprite spr_border(texture1);
    spr_border.setPosition({ 0, 768 - 192 });

    const sf::Texture texture2("images\\button.png");
    sf::Sprite spr_but_action(texture2);

    const sf::Texture texture3("images\\cursor_def.png");
    sf::Sprite cursor_def(texture3);

    const sf::Texture texture4("images\\cursor_my.png");
    sf::Sprite cursor_my(texture4);
    
    // Перечисление всех типов танков с учетом порядка их компонент
    addUnitSprite("unicorn", "images\\unicorn.png");
    addUnitSprite("unicorn_attacker", "images\\unicorn.png");
    addUnitSprite("unicorn_harvester", "images\\unicorn.png");
    addUnitSprite("unicorn_healer", "images\\unicorn.png");
    addUnitSprite("unicorn_radar", "images\\unicorn.png");

    addUnitSprite("unicorn_harvesterattacker", "images\\unicorn.png");
    addUnitSprite("unicorn_harvesterhealer", "images\\unicorn.png");
    addUnitSprite("unicorn_attackerhealer", "images\\unicorn.png");
    addUnitSprite("unicorn_harvesterradar", "images\\unicorn.png");
    addUnitSprite("unicorn_healerradar", "images\\unicorn.png");
    addUnitSprite("unicorn_attackerradar", "images\\unicorn.png");

    addUnitSprite("unicorn_harvesterattackerhealer", "images\\unicorn.png");
    addUnitSprite("unicorn_harvesterhealerradar", "images\\unicorn.png");
    addUnitSprite("unicorn_harvesterattackerradar", "images\\unicorn.png");
    addUnitSprite("unicorn_attackerhealerradar", "images\\unicorn.png");

    addUnitSprite("unicorn_harvesterattackerhealerradar", "images\\unicorn.png");

    addUnitSprite("portal", "images\\portal.png");
    addUnitSprite("crystal", "images\\crystal.png");
    addUnitSprite("monster1", "images\\monster1.png");
    addUnitSprite("monster2", "images\\monster2.png");
    addUnitSprite("monster3", "images\\monster3.png");
    addUnitSprite("lair", "images\\lair.png");
    
    // Здесь можно использовать автоподгрузку, если есть запрос от UI на спрайт
    addActionSprite("build", "images\\action_build.png");
    addActionSprite("upgrade_hp", "images\\action_upgrade_hp.png");
    addActionSprite("upgrade_count", "images\\action_upgrade_count.png");
    addActionSprite("make_harvester", "images\\action_make_harvester.png");
    addActionSprite("make_healer", "images\\action_make_healer.png");
    addActionSprite("make_attacker", "images\\action_make_attacker.png");
    addActionSprite("make_radar", "images\\action_make_radar.png");
    addActionSprite("do_radar", "images\\action_do_radar.png");
    addActionSprite("upgrade_harvester", "images\\action_upgrade_harvester.png");
    addActionSprite("upgrade_attacker", "images\\action_upgrade_attacker.png");
    addActionSprite("upgrade_healer", "images\\action_upgrade_healer.png");

    view.setSize({ VIEW_SIZE_X, VIEW_SIZE_Y });
    view.setViewport(sf::FloatRect({0.0,0.0},{1.0,0.75}));

    const sf::Font font("arial.ttf");

    // Все надписи готовим на основе шрифта
    sf::Text text_caption(font, "", 24);
    text_caption.setFillColor(sf::Color::White);
    
    sf::Text text_ok(font, "", 24);
    text_ok.setFillColor(sf::Color::White);
    text_ok.setString("OK");

    sf::Text text_action(font, "", 16);
    text_action.setPosition({ 1024 - 400 + 8, 720 });

    sf::Text text_hp(font, "", 18);

    sf::Text text_task(font, "", 22);
    text_task.setPosition({ 16, 720 });

    sf::Text text_info(font, "", 16);
    text_info.setFillColor(sf::Color::White);

    sf::Text text_timer(font, "", 20);
    text_timer.setFillColor(sf::Color::White);

    sf::Text text_resource(font, "", 18);
    
    sf::Text text_progress(font, "", 24);
    text_progress.setFillColor(sf::Color::White);
    text_progress.setPosition({ 1024 - 400 / 2 - 16, 576 + 132 });

    sf::Text text_gameover(font, "", 24);
    text_gameover.setPosition({ 512 - 80, 270 });

    // Прямоугольники интерфейса
    sf::RectangleShape selector;
    selector.setOutlineThickness(3);
    selector.setFillColor(sf::Color::Transparent);
    
    sf::RectangleShape rect_progress_border;
rect_progress_border.setOutlineThickness(2);
rect_progress_border.setOutlineColor(sf::Color::White);
rect_progress_border.setPosition({ 1024 - 400 + 8, 576 + 132 });
rect_progress_border.setSize({ 400-16, 32 });
rect_progress_border.setFillColor(sf::Color::Transparent);

sf::RectangleShape laser;

sf::RectangleShape rect_progress;
rect_progress.setOutlineThickness(0);
rect_progress.setPosition(rect_progress_border.getPosition());
rect_progress.setFillColor(sf::Color(217, 138, 255));

rect_health.setSize({ 4, 4 });
rect_health.setOutlineThickness(1);
rect_health.setOutlineColor(sf::Color{ 64,64,64 });

sf::RectangleShape textback;
textback.setOutlineThickness(1);
textback.setOutlineColor(sf::Color(192,192,192));
textback.setFillColor(sf::Color{ 40, 40, 40, 192 });

sf::RectangleShape endgameback;
endgameback.setOutlineThickness(2);
endgameback.setFillColor(sf::Color{ 40, 40, 40, 192 });
endgameback.setPosition({ 512 - 150, 258 });
endgameback.setSize({ 300, 100 });

// Звуки лазера и старта
sf::Music effect_fire("sounds\\laser.ogg");
effect_fire.setVolume(0.0f);
effect_fire.setLooping(true);
effect_fire.play();

sf::SoundBuffer effect_start_buffer("sounds\\start.ogg");
sf::Sound effect_start(effect_start_buffer);

addTerrainSprite(Terrain::Ground, "images\\terrain_ground.png");
addTerrainSprite(Terrain::Water, "images\\terrain_water.png");
addTerrainSprite(Terrain::Forest, "images\\terrain_forest.png");
addTerrainSprite(Terrain::Road, "images\\terrain_road.png");

addTreeSprite(TreeType::Bottom, "images\\tree_bottom.png");
addTreeSprite(TreeType::BottomLeft, "images\\tree_bottom_left.png");
addTreeSprite(TreeType::BottomRight, "images\\tree_bottom_right.png");
addTreeSprite(TreeType::Top, "images\\tree_top.png");
addTreeSprite(TreeType::TopLeft, "images\\tree_top_left.png");
addTreeSprite(TreeType::TopRight, "images\\tree_top_right.png");
addTreeSprite(TreeType::Left, "images\\tree_left.png");
addTreeSprite(TreeType::Right, "images\\tree_right.png");
addTreeSprite(TreeType::WaterTopLeft, "images\\water_top_left.png");
addTreeSprite(TreeType::WaterTop, "images\\water_top.png");
addTreeSprite(TreeType::WaterTopRight, "images\\water_top_right.png");
addTreeSprite(TreeType::WaterLeft, "images\\water_left.png");
addTreeSprite(TreeType::WaterRight, "images\\water_right.png");
addTreeSprite(TreeType::WaterBottomLeft, "images\\water_bottom_left.png");
addTreeSprite(TreeType::WaterBottom, "images\\water_bottom.png");
addTreeSprite(TreeType::WaterBottomRight, "images\\water_bottom_right.png");
addTreeSprite(TreeType::RoadHorz, "images\\road_horz.png");
addTreeSprite(TreeType::RoadVert, "images\\road_vert.png");
addTreeSprite(TreeType::RoadTopLeft, "images\\road_top_left.png");
addTreeSprite(TreeType::RoadTopRight, "images\\road_top_right.png");
addTreeSprite(TreeType::RoadBottomLeft, "images\\road_bottom_left.png");
addTreeSprite(TreeType::RoadBottomRight, "images\\road_bottom_right.png");

// Цвета территорий и лазеров
color_terrains[Terrain::Ground] = sf::Color(105, 149, 19);
color_terrains[Terrain::Water] = sf::Color(0, 206, 228);
color_terrains[Terrain::Forest] = sf::Color(25, 103, 39);
color_terrains[Terrain::Road] = sf::Color(228, 218, 171);

color_lasers[LaserType::Harvest] = sf::Color(0, 255, 255);
color_lasers[LaserType::Attack] = sf::Color(255, 0, 0);
color_lasers[LaserType::Heal] = sf::Color(240, 255, 0);

Animation laser_apply("images\\laser_apply.png", 30, 34, 16, 16);
laser_apply.setOrigin({ 15,17 });
laser_apply.play();

// Шейдеры яркости и обесцвечивания
sf::Shader shader_gray;
sf::Shader shader_bright;

shader_gray.loadFromFile("shaders\\gray.frag", sf::Shader::Type::Fragment);
shader_bright.loadFromFile("shaders\\bright.frag", sf::Shader::Type::Fragment);

shader_gray.setUniform("texture", sf::Shader::CurrentTexture);
shader_bright.setUniform("texture", sf::Shader::CurrentTexture);

sf::Clock clock;
float progress;
bool modeendgame = false;
scene = Scene::Menu;

if (std::filesystem::exists(exedir + "\\developer.json"))
    game.loadDeveloperConfig(exedir + "\\developer.json");

const int LEVEL_COUNT = 4;

// Крутим цикл игры
while (window.isOpen())
{
    auto mousePos = sf::Mouse::getPosition(window);

    sf::Sprite* cursor = &cursor_def;

    float dt = clock.getElapsedTime().asSeconds();
    clock.restart();

    // Обработка сцены меню
    if (scene == Scene::Menu) {
        // Дублирование с определением в клике (с учетом доп. кнопки выхода + 1)
        textback.setSize({ 240, 40 });
        for (int i = 0; i < LEVEL_COUNT + 1; i++) {
            textback.setPosition({ 512 - 120, (float)(350 + 64 * i) });
            if (textback.getGlobalBounds().contains({ (float)mousePos.x, (float)mousePos.y }))
                cursor = &cursor_my;
        }
        // Получаем все события от окна
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>()) window.close();
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)	window.close();
            };
            if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonReleased>())
            {
                textback.setSize({ 240, 40 });
                for (int i = 0; i < LEVEL_COUNT; i++) {
                    textback.setPosition({ 512 - 100, (float)(350 + 64 * i) });
                    if (textback.getGlobalBounds().contains({ (float)mousePos.x, (float)mousePos.y })) {
                        // Для выбора игры - переходим на сцену задания и загружаем игру
                        scene = Scene::Task;
                        modeendgame = false;
                        loadGame(i);
                    }
                }
                textback.setPosition({ 512 - 100, (float)(350 + 64 * LEVEL_COUNT) });
                if (textback.getGlobalBounds().contains({ (float)mousePos.x, (float)mousePos.y })) window.close();
            }
        }
    }
    // Для сцены задания
    if (scene == Scene::Task) {
    while (const std::optional event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>()) window.close();
        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
        {
            // Переход к игре по нажатию на кнопку Esc
            if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)	scene = Scene::Menu;
        };
        if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonReleased>())
        {
            // Переход к игре по нажатию на кнопку ОК
            textback.setPosition({ 512 - 40, 400 });
            textback.setSize({ 80, 30 });
            if (textback.getGlobalBounds().contains({ (float)mousePos.x, (float)mousePos.y })) scene = Scene::Game;
        }
    }
    }
    // Далее строго сцена игры
    if (scene == Scene::Game) {
    // Дублирование с определением при нажатии
    if (mousePos.x > 256) {
        window.setView(view);
        sf::Vector2f worldpos = window.mapPixelToCoords(mousePos);
        window.setView(window.getDefaultView());

        int uid;
        if (game.findUnitAt(worldpos.x, worldpos.y, &uid))
            if (!game.isFog(game.getUnitByUID(uid).getXY().x, game.getUnitByUID(uid).getXY().y))
                cursor = &cursor_my;
    }
    effect_fire.setVolume(game.getLaserCount() > 0 ? 100.0f : 0.0f);

    while (const std::optional event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
            window.close();
        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
        {
            if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
                // Инициализация механизма завершения уровня - отключение звуковых эффектов, дублирование кода
                effect_fire.setVolume(0.0f);
                effect_start.stop();
                scene = Scene::Menu;
            }
            if (keyPressed->scancode == sf::Keyboard::Scancode::NumpadMinus) {
                if (tekscale < 6) {
                    tekscale++;
                    updateScale();
                }
            }
            if (keyPressed->scancode == sf::Keyboard::Scancode::NumpadPlus) {
                if (tekscale > 0) {
                    tekscale--;
                    updateScale();
                }
            }
        };
        // Для диалога завершения игры - выход к сцене меню
        if (modeendgame) {
            if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonReleased>()) {
                textback.setPosition({ 512 - 40, 320 });
                textback.setSize({ 80, 30 });
                if (textback.getGlobalBounds().contains({ (float)mousePos.x, (float)mousePos.y })) scene = Scene::Menu;
            }
        }
        else {
            if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>())
            {
                // Зона игры
                if (mousePressed->position.y < 576.0f) {
                    window.setView(view);
                    sf::Vector2f worldpos = window.mapPixelToCoords(mousePressed->position);
                    window.setView(window.getDefaultView());

                    // Выделение юнита
                    if (mousePressed->button == sf::Mouse::Button::Left)
                    {
                        int uid;
                        if (game.findUnitAt(worldpos.x, worldpos.y, &uid))
                            if (!game.isFog(game.getUnitByUID(uid).getXY().x, game.getUnitByUID(uid).getXY().y)) {
                                selected_uid = uid;
                            }
                    }

                    // Команда движения юнита
                    if (mousePressed->button == sf::Mouse::Button::Right)
                    {
                        if (selected_uid)
                            if (game.getUnitByUID(*selected_uid).isComponent<ComponentUnicorn>()) {
                                game.setTargetToUnit(*selected_uid, worldpos.x / BLOCKW, worldpos.y / BLOCKH);
                                effect_start.play();
                                started_galop_uid = *selected_uid;
                            }
                    }
                }
                // Зона действий
                else {
                    if (selected_uid) {
                        std::string msgcode = "";
                        // Разрешаем действия только если юнит не работает над действием в данный момент
                        if (!game.getUnitByUID(*selected_uid).isWorkingTask()) {
                            auto actions = game.getUnitByUID(*selected_uid).getActions();
                            for (int i = 0; i < actions.size(); i++) {
                                spr_but_action.setPosition(getActionButtonPos(i));
                                if (spr_but_action.getGlobalBounds().contains({ (float)mousePressed->position.x, (float)mousePressed->position.y })) {
                                    if (!game.getUnitByUID(*selected_uid).canSendAction(actions[i], &msgcode)) {
                                        text_action.setString(texts.getSfmlStr(msgcode));
                                        counter_errmsg.upset(1.0f);
                                    }
                                    else
                                        game.sendUnitAction(*selected_uid, actions[i]);
                                }
                            }
                        }
                    }
                }
            };
        }
    }
    
    if (!modeendgame) {
        // Прокрутка карты
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left)) {
            view.move({ -SCROLLSPEED * BLOCKW * dt, 0 });
            fixCameraPosition();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right)) {
            view.move({ +SCROLLSPEED * BLOCKW * dt, 0 });
            fixCameraPosition();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up)) {
            view.move({ 0, -SCROLLSPEED * BLOCKH * dt });
            fixCameraPosition();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Down)) {
            view.move({ 0, SCROLLSPEED * BLOCKH * dt });
            fixCameraPosition();
        }

        laser_apply.update(dt);
        game.update(dt);
        if (!game.isUnitExist(*selected_uid)) selected_uid = std::nullopt;

        updateMiniMap(game);
    }

    counter_errmsg.update(dt);
    counter_endgame.update(dt);

    if (effect_start.getStatus() == sf::SoundSource::Status::Playing)
        if (game.isUnitExist(started_galop_uid))
            if (!game.getUnitByUID(started_galop_uid).isTargeted()) effect_start.stop();

    if (counter_endgame.onceReachNol()) {
        // Инициализация механизма завершения уровня - отключение звуковых эффектов
        modeendgame = true;
        effect_fire.setVolume(0.0f);
        effect_start.stop();
    }

    if (!modeendgame)
        if (game.isGameOver())
            if (!counter_endgame.isActive()) counter_endgame.upset(2.0f);

        }
        window.clear();

        // Для сцены меню - вывод меню и фона
        if (scene == Scene::Menu) {
            window.draw(spr_intro);

            textback.setSize({ 240, 40 });
            for (int i = 0; i < LEVEL_COUNT; i++) {
                textback.setPosition({ 512-120, (float)(350 + 64 * i) });
                window.draw(textback);

                text_info.setString(texts.getSfmlStr("Name_Level_" + std::to_string(i)));
                text_info.setPosition({ 512 - text_info.getLocalBounds().size.x/2, (float)(350 + 64 * i) + 8 });
                window.draw(text_info);
            }
            textback.setPosition({ 512 - 120, (float)(350 + 64 * LEVEL_COUNT) });
            window.draw(textback);

            text_info.setString(texts.getSfmlStr("Text_Quit"));
            text_info.setPosition({ 512 - text_info.getLocalBounds().size.x / 2, (float)(350 + 64 * LEVEL_COUNT) + 8 });
            window.draw(text_info);
        }
        // Для сцены задачи и игры - вывод территорий
        if ((scene == Scene::Task) || (scene == Scene::Game)) {
            window.setView(view);

                for (int i = 0; i < game.getWidth(); i++)
                    for (int j = 0; j < game.getHeight(); j++)
                        if (!game.isFog(i, j)) {
                            if (auto treeblock = treebuilder.getTreeType(i, j)) {
                                // Этот трюк нужен, чтобы сначала вывелись территории и фрагменты леса нижние, а потом - верхние, закрывающие пони
                                if (!((*treeblock == TreeType::Top) || (*treeblock == TreeType::TopLeft) || (*treeblock == TreeType::TopRight))) {
                                    spr_trees[*treeblock]->setPosition(sf::Vector2f(i * BLOCKW, j * BLOCKH));
                                    window.draw(*spr_trees[*treeblock]);
                                }
                                else {
                                    if (spr_terrains.count(game.getMap(i, j)) > 0) {
                                        spr_terrains[game.getMap(i, j)]->setPosition(sf::Vector2f(i * BLOCKW, j * BLOCKH));
                                        window.draw(*spr_terrains[game.getMap(i, j)]);
                                    }
                                }
                            }
                            else
                            if (spr_terrains.count(game.getMap(i, j)) > 0) {
                                spr_terrains[game.getMap(i, j)]->setPosition(sf::Vector2f(i * BLOCKW, j * BLOCKH));
                                window.draw(*spr_terrains[game.getMap(i, j)]);
                            }
                        }

                // Вывод юнитов
                for (int i = 0; i < game.getUnitCount(); i++)
                    if (!game.isFog(game.getUnit(i).getXY().x, game.getUnit(i).getXY().y))
                        if (spr_units.count(game.getUnit(i).getCode()) > 0) {
                            spr_units[game.getUnit(i).getCode()]->setPosition(game.getUnit(i).getView());
                            spr_units[game.getUnit(i).getCode()]->setScale({ game.getUnit(i).getLastMoving() == Moving::Left ? -1.0f : 1.0f,1 });
                            window.draw(*spr_units[game.getUnit(i).getCode()]);
                        }

                // И здесь вывод только верхних фрагментов леса
                for (int i = 0; i < game.getWidth(); i++)
                    for (int j = 0; j < game.getHeight(); j++)
                        if (!game.isFog(i, j)) {
                            if (auto treeblock = treebuilder.getTreeType(i, j)) {
                                if ((*treeblock == TreeType::Top) || (*treeblock == TreeType::TopLeft) || (*treeblock == TreeType::TopRight)) {
                                    spr_trees[*treeblock]->setPosition(sf::Vector2f(i * BLOCKW, j * BLOCKH));
                                    window.draw(*spr_trees[*treeblock]);
                                }
                            }
                        }

            // Полоски здоровья выводим после юнитов
                for (int i = 0; i < game.getUnitCount(); i++)
                    if (!game.isFog(game.getUnit(i).getXY().x, game.getUnit(i).getXY().y))
                        drawHealthRectAt(window, game.getUnit(i).getHealthPerMax(), game.getUnit(i).getSizeView().x,
                            game.getUnit(i).getView().x - game.getUnit(i).getSizeView().x / 2.0f,
                            game.getUnit(i).getView().y - game.getUnit(i).getSizeView().y / 2.0f - 10);

            // Лазеры выводим после всего
            for (int i = 0; i < game.getLaserCount(); i++)
                drawLaserFromTo(window, laser, game.getLaser(i), laser_apply);

            if (selected_uid) {
                if (game.getUnitByUID(*selected_uid).isComponent<ComponentResource>())
                    selector.setOutlineColor(sf::Color::Blue);
                else
                    if (game.getUnitByUID(*selected_uid).isComponent<ComponentEnemy>())
                        selector.setOutlineColor(sf::Color::Red);
                    else
                        selector.setOutlineColor(sf::Color::Green);
                selector.setPosition(game.getUnitByUID(*selected_uid).getView());
                selector.setSize(game.getUnitByUID(*selected_uid).getSizeView());
                selector.setOrigin(game.getUnitByUID(*selected_uid).getSizeView() / 2.0f);
                window.draw(selector);
            }

            window.setView(window.getDefaultView());
            // Далее рисуем управлющие элементы панели
            window.draw(spr_border);

            minimap.drawTo(&window);

            textback.setPosition({ 6, 6 });
            textback.setSize({ 240, 30 });
            window.draw(textback);

            // Информация по ресурсам и танкам
            text_resource.setString(SfmlTools::utf2text(texts.getStr("Text_Energy") + " " + std::to_string(game.getEnergy())));
            text_resource.setPosition({ 10, 10 });
            text_resource.setFillColor(sf::Color{ 162, 231, 255 });
            window.draw(text_resource);
            text_resource.setString(SfmlTools::utf2text(texts.getStr("Text_UnicornCount") + " " + game.getUnicornCountInfo()));
            text_resource.setPosition({ 256 / 2 + 12, 10 });
            text_resource.setFillColor(sf::Color::White);
            window.draw(text_resource);

            if (selected_uid) {
                // Информация по юниту
                textback.setSize({ 240, 192 });
                textback.setPosition({ 512 - textback.getSize().x/2 - 64, 768 - textback.getSize().y });
               // window.draw(textback);

                const GameUnit& selunit = game.getUnitByUID(*selected_uid);
                drawHealthRectAt(window, selunit.getHealthPerMax(), 48, textback.getPosition().x + 12, textback.getPosition().y + 64);

                if (spr_icons.count(selunit.getCode()) > 0) {
                    spr_icons[selunit.getCode()]->setPosition({ textback.getPosition().x + 12 + 48 / 2, textback.getPosition().y + 34 });
                    window.draw(*spr_icons[selunit.getCode()]);
                }

                text_caption.setString(texts.getSfmlStr("Unit_" + selunit.getCaption()));
                text_caption.setPosition({ textback.getPosition().x + 78, textback.getPosition().y + 18 });
                window.draw(text_caption);

                text_hp.setString(selunit.getHealthInfo());
                text_hp.setPosition({ textback.getPosition().x + 78, textback.getPosition().y + 54 });
                text_hp.setFillColor(getColorByHPNorm(selunit.getHealthPerMax()));
                window.draw(text_hp);

                text_info.setString(texts.getSfmlStrReplacedConsts(selunit.getComponentsInfo()));
                text_info.setPosition({ textback.getPosition().x + 12, textback.getPosition().y + 92 });
                window.draw(text_info);
                
                // Действия юнита
                textback.setSize({ 400, 192 });
                textback.setPosition({ 1024 - textback.getSize().x, 768 - textback.getSize().y });
               // window.draw(textback);

                std::string current_action_code;
                if (selunit.isWorkingTask(&progress, &current_action_code)) {
                    window.draw(rect_progress_border);
                    rect_progress.setSize({ (400-16) * progress, 32 });
                    window.draw(rect_progress);
                    text_progress.setString(std::to_string((int)(100 * progress)) + "%");
                    window.draw(text_progress);
                    spr_actions[current_action_code]->setPosition({ (float)(textback.getPosition().x + 200 - 32), textback.getPosition().y + 8 });
                    window.draw(*spr_actions[current_action_code]);
                }
                else {
                    auto actions = selunit.getActions();
                    for (int i = 0; i < actions.size(); i++) {
                        spr_but_action.setPosition(getActionButtonPos(i));
                        bool isover = spr_but_action.getGlobalBounds().contains({ (float)mousePos.x,(float)mousePos.y });
                        std::string msgcode = "";
                        window.draw(spr_but_action);
                        if (spr_actions.count(actions[i].code) > 0) {
                            // Вывод действий либо яркими для позиции под курсором, либо серыми для недоступных
                            spr_actions[actions[i].code]->setPosition({ getActionButtonPos(i).x + 4,getActionButtonPos(i).y + 4 });
                            if (!selunit.canSendAction(actions[i], &msgcode))
                                window.draw(*spr_actions[actions[i].code], &shader_gray);
                            else
                                if (isover)
                                    window.draw(*spr_actions[actions[i].code], &shader_bright);
                                else
                                    window.draw(*spr_actions[actions[i].code]);
                        }

                        if (isover) cursor = &cursor_my;

                        if ((isover) && (!counter_errmsg.isActive())) {
                            text_action.setString(SfmlTools::utf2text(
                                texts.getStr("Action_" + actions[i].caption) + "\n" + texts.getStr("Text_Energy") + " " + std::to_string(actions[i].energy)));
                            text_action.setFillColor(sf::Color::White);
                            window.draw(text_action);
                        }
                    }
                }
            }

            // Вывод сообщения об ошибке, если есть
            if (counter_errmsg.isActive()) {
                text_action.setFillColor(sf::Color::Red);
                window.draw(text_action);
            }

            // Вывод таймера, если он есть
            if (auto stimer = game.getTimerStr()) {
                textback.setPosition({ 1024 - 144, 4 });
                textback.setSize({ 140, 36 });
                window.draw(textback);

                text_timer.setString(SfmlTools::utf2text(texts.getStr("Text_Timer") + " " + *stimer));
                text_timer.setPosition({ 1024 - 144 + 10, 8 });
                window.draw(text_timer);
            }


            // Для режима завершения игры - вывод диалога
            if (modeendgame) {
                if (game.isFail()) {
                    endgameback.setOutlineColor(sf::Color::Red);
                    text_gameover.setFillColor(sf::Color::Red);
                    text_gameover.setString(texts.getSfmlStr("Msg_Fail"));
                }
                else {
                    endgameback.setOutlineColor(sf::Color::Green);
                    text_gameover.setFillColor(sf::Color::Green);
                    text_gameover.setString(texts.getSfmlStr("Msg_Win"));
                }
                window.draw(endgameback);
                window.draw(text_gameover);

                textback.setPosition({ 512 - 40, 320 });
                textback.setSize({ 80, 30 });
                window.draw(textback);

                text_ok.setPosition({ 512 - 20, 320 });
                window.draw(text_ok);
            }            

            // Для сцены задания - вывод диалога
            if (scene == Scene::Task) {
                textback.setPosition({512 - 200, 286 });
                textback.setSize({ 400, 150 });
                window.draw(textback);

                text_task.setString(texts.getSfmlStr("Text_Task")+"\n"+SfmlTools::utf2text(game.getTaskText()));
                text_task.setPosition({ 512 - 190, 290 });
                window.draw(text_task);

                textback.setPosition({ 512 - 40, 400 });
                textback.setSize({ 80, 30 });
                window.draw(textback);

                text_ok.setPosition({ 512 - 20, 400 });
                window.draw(text_ok);
            }
        }
        // Вывод курсора
        int delta = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) ? 4 : 0;
        cursor->setPosition({ (float)mousePos.x + delta,(float)mousePos.y + delta });
        window.draw(*cursor);

        window.display();
    }

    return 0;
}
