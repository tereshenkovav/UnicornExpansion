#pragma warning (disable: 4275)

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <filesystem>
#include <string>
#include <filesystem>
#include <map>
#include "SfmlGameEngine/Animation.h"

#include "Game.h"
#include "GameUnit.h"
#include "HelperCppClasses/Countdown.h"
#include "MiniMap.h"
#include "SfmlGameEngine/SfmlTools.h"
#include "UnitFactory.h"
#include "ComponentUnicorn.h"
#include "ComponentMeleeEnemy.h"
#include "ComponentEnemy.h"
#include "ComponentResource.h"
#include "ComponentHarvester.h"
#include "SubTerrainBuilder.h"
#include "FogBuilder.h"
#include "HelperCppClasses/StringTools.h"
#include "ClickerCounter.h"
#include "UnitSelector.h"
#include "SfmlGameEngine/Engine.h"
#include "SceneMainMenu.h"
#include "SceneTask.h"

#pragma comment (lib, "sfml-graphics.lib")
#pragma comment (lib, "sfml-system.lib")
#pragma comment (lib, "sfml-window.lib")
#pragma comment (lib, "sfml-audio.lib")
#pragma comment (lib, "sfml-main-s.lib")
#pragma comment (lib, "zetscript-2-1-0-static.lib")
#pragma comment (lib, "jsoncpp_static.lib")

/*
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
std::vector<std::unique_ptr<sf::Sprite>> spr_mushrooms;
std::map<Terrain, sf::Color> color_terrains;
std::map<LaserType, sf::Color> color_lasers;
std::map<LaserType, sfge::Animation*> anim_lasers;
std::map<TerrainSubType, std::unique_ptr<sf::Sprite>> spr_trees;
std::vector<std::unique_ptr<sf::SoundBuffer>> soundbuffers;
std::vector<std::unique_ptr<sf::Sound>> snd_unicorn_clicks;
std::map<AudioEffect, std::unique_ptr<sf::Sound>> snd_audioeffects;
sf::View view;
Countdown counter_errmsg;
MiniMap minimap;
Texts texts;
Game game;
SubTerrainBuilder stbuilder;
FogBuilder fogbuilder;
// Управление выделенными юнитами
UnitSelector selector;
int started_galop_uid;
Countdown counter_endgame;
int tekscale;
sf::RectangleShape rect_pblocks;
std::optional<sfge::Animation> current_teleportation_effect;
bool mouseholdedonmap = false;
float globalt = 0.0f;

// Сдвиговые значения для маркеров способностей
std::array<int, 8> marker_dx = { 11, 0, 11, 0, 11, 0, 11, 0 };
std::array<int, 8> marker_dy = { 0, 0, 11, 11, 64 - 22, 64 - 22, 64 - 11, 64 - 11 };

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

void addSubTerrainSprite(TerrainSubType ttype, const std::string& filename) {
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

// Добавление аудиоэффекта
void addAudioEffect(AudioEffect code, const std::string& filename) {
    soundbuffers.push_back(std::make_unique<sf::SoundBuffer>(filename));
    snd_audioeffects[code] = std::make_unique<sf::Sound>(*soundbuffers.back());
}

// Обновление мини-карты
void updateMiniMap(const Game & game) {
    for (int i = 0; i < game.getWidth(); i++)
        for (int j = 0; j < game.getHeight(); j++) {
            if (game.isFog(i, j))
                minimap.setCellColor(i, j, sf::Color::Black);
            else {
                if (color_terrains.count(game.getMap(i, j)) > 0) minimap.setCellColor(i, j, color_terrains[game.getMap(i, j)]);
                // Затемнение грибами, только рельефа
                int stage = game.getMushrooms(i, j).size();
                // Здесь зависимость прозрачности от стадии грибов - считаем, что максимальная плотность это 4
                if (stage>0) minimap.addCellColor(i, j, sf::Color{ 160,56,255,uint8_t(64+32*stage)});
            }
        }

    // Работаем со всеми юнитами, без деления на свои и чужие
    for (int i = 0; i < game.getUnitCount(); i++)
        for (int dx = 0; dx < game.getUnit(i).getSize().x; dx++)
            for (int dy = 0; dy < game.getUnit(i).getSize().y; dy++) {
                if (game.isFog(game.getUnit(i).getXY().x + dx, game.getUnit(i).getXY().y + dy)) continue;
                sf::Color color;
                if (selector.isUnitSelected(game.getUnit(i).getUID())) {
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

float getScale05per20() {
    return 0.5f + tekscale * 0.25f;
}

// Рисование лазера и его анимации в точке приложения
void drawLaserFromTo(sf::RenderWindow& window, sf::Sprite spr_laz, const Laser& laz) {
    sf::Vector2f dir = laz.pos2 - laz.pos1;
    spr_laz.setColor(sfge::SfmlTools::getColorAsBright(color_lasers[laz.type],0.9+0.2*sin(4.0f*M_PI*globalt+laz.timeshift)));
    spr_laz.setPosition(laz.pos1);
    spr_laz.setRotation(dir.angle());
    spr_laz.setScale({ dir.length()/128.0f,1.0f });
    window.draw(spr_laz);

    anim_lasers[laz.type]->setColor(color_lasers[laz.type]);
    anim_lasers[laz.type]->setPosition(laz.pos2);
    window.draw(*anim_lasers[laz.type]);
}

void fixCameraPosition() {
    if (view.getCenter().x < view.getSize().x / 2.0f) view.setCenter({ view.getSize().x / 2.0f, view.getCenter().y });
    if (view.getCenter().x > game.getWidth() * BLOCKW - view.getSize().x / 2.0f) view.setCenter({ game.getWidth() * BLOCKW - view.getSize().x / 2.0f, view.getCenter().y });
    if (view.getCenter().y < view.getSize().y / 2.0f) view.setCenter({ view.getCenter().x, view.getSize().y / 2.0f });
    if (view.getCenter().y > game.getHeight() * BLOCKH - view.getSize().y / 2.0f) view.setCenter({ view.getCenter().x, game.getHeight() * BLOCKH - view.getSize().y / 2.0f });
}

void updateScale() {
    view.setSize({ VIEW_SIZE_X * getScale05per20(), VIEW_SIZE_Y * getScale05per20() });
    minimap.setWindowSize(view.getSize().x, view.getSize().y);
    fixCameraPosition();
}

void drawProgressRectsAt(sf::RenderWindow& window, float perc, float basew, float x, float y, sf::Color color) {
    rect_pblocks.setFillColor(color);
    int cntrect = ((int)(basew * perc) / (rect_pblocks.getSize().x + 2)) + 1;
    for (int j = 0; j < cntrect; j++) {
        rect_pblocks.setPosition({ x + j * (rect_pblocks.getSize().x + 2), y });
        window.draw(rect_pblocks);
    }
}

// Загрузчик игры из файлов
void loadGame(int leveln) {
    bool paramok = game.loadConfigs();
    bool mapok = game.loadMap("levels/level" + std::to_string(leveln) + ".map");
    bool scriptok = game.loadScript("levels/level" + std::to_string(leveln) + ".script");
    
    game.update(0.0); // Первичная инициализация для тумана войны
    tekscale = 2.0;
    updateScale();

    // Обновление камеры здесь дублировано для того, чтобы установить её до вызова основного цикла, пока показано задание
    if (auto newvp = game.getOnceNewViewPoint())
        view.setCenter({ (float)(*newvp).x * BLOCKW, (float)(*newvp).y * BLOCKH });

    selector.unSelectAll();

    minimap.prepareMiniMap(8, 768 - 192 + 8, 192 - 16, 192 - 16, game.getWidth(), game.getHeight(),
        VIEW_SIZE_X, VIEW_SIZE_Y, game.getWidth() * BLOCKW, game.getHeight() * BLOCKH);
    updateMiniMap(game);

    stbuilder.updateByGame(game);
    fogbuilder.initByGame(game);

}
*/
int main(int argc, char* argv[])
{
    srand(time(NULL));

    std::filesystem::path exepath(argv[0]);
    std::string exedir = exepath.parent_path().string();

    // Установка текущего каталога
    // При ручном указании каталога
    if (argc > 1)
        std::filesystem::current_path(std::string(argv[1]));
    else
        // Для корректной работы внутри AppImage
        if (hasEnding(std::filesystem::current_path().string(), "/usr"))
            std::filesystem::current_path("data");
    // Вариант по умолчанию - data в каталоге исполняемого файла
        else
            std::filesystem::current_path(exedir + "/data");
        
    sfge::Engine engine(1024, 768);
    engine.loadTexts("strings.txt");
    engine.loadColors("colors.dat");
    engine.setCaption(engine.getTexts().getStr("Text_GameCaption"));
    engine.setIcon("images/icon.png");
    engine.loadDefaultFont("arial.ttf");
    engine.loadDefaultCursor("images/cursor_def.png");
    engine.addCursor(1, "images/cursor_my.png");
    engine.Run(std::make_shared<SceneMainMenu>());

    /*
    texts.loadFromFile("strings.txt");

    // Загрузка всех ресурсов
    
    const sf::Texture texture1("images/border.png");
    sf::Sprite spr_border(texture1);
    spr_border.setPosition({ 0, 768 - 192 });

    const sf::Texture texture2("images/button.png");
    sf::Sprite spr_but_action(texture2);

    const sf::Texture texture3("images/cursor_def.png");
    sf::Sprite cursor_def(texture3);

    const sf::Texture texture4("images/cursor_my.png");
    sf::Sprite cursor_my(texture4);

    const sf::Texture texture5("images/undo.png");
    sf::Sprite undo(texture5);
    undo.setPosition({ 1024 - 36 - 10, VIEW_SIZE_Y + 130 });

    const sf::Texture texture6("images/marker.png");
    sf::Sprite marker(texture6);
    marker.setOrigin({ 5, 5 });
    
    std::map<std::string, sf::Color> components_color;
    components_color["healer"] = sf::Color(240, 255, 0);
    components_color["attacker"] = sf::Color(255, 0, 0);
    components_color["harvester"] = sf::Color(0, 64, 255);
    components_color["detoxer"] = sf::Color(208, 0, 220);
    components_color["radar"] = sf::Color(255, 255, 255);
    components_color["shield"] = sf::Color(107, 230, 255);

   // Используется загрузка каталога в целом, можно вынести как процедуру
    std::string pathload = "images/units/";
    for (auto& filename : std::filesystem::directory_iterator(pathload)) {
        auto str = filename.path().string();
        replaceFirstString(str, pathload, "");
        replaceFirstString(str, ".png", "");
        addUnitSprite(str, filename.path().string());
    }

    // Используется загрузка каталога в целом, можно вынести как процедуру
    pathload = "images/actions/";
    for (auto& filename : std::filesystem::directory_iterator(pathload)) {
        auto str = filename.path().string();
        replaceFirstString(str, pathload, "");
        replaceFirstString(str, ".png", "");
        addActionSprite(str, filename.path().string());
    }

    // Используется загрузка каталога в целом, можно вынести как процедуру
    pathload = "images/mushrooms/";
    for (auto& filename : std::filesystem::directory_iterator(pathload)) {
        textures.push_back(std::make_unique<sf::Texture>(filename));
        spr_mushrooms.push_back(std::make_unique<sf::Sprite>(*textures.back()));
        spr_mushrooms.back()->setOrigin({ textures.back()->getSize().x / 2.0f, textures.back()->getSize().y / 2.0f });
    }

    view.setSize({ VIEW_SIZE_X, VIEW_SIZE_Y });
    view.setViewport(sf::FloatRect({ 0.0,0.0 }, { 1.0,0.75 }));

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
        
    sf::Text text_timer(font, "", 20);
    text_timer.setFillColor(sf::Color::White);

    sf::Text text_resource(font, "", 18);
    
    sf::Text text_progress(font, "", 24);
    text_progress.setFillColor(sf::Color::White);
    text_progress.setPosition({ 1024 - 400 / 2 - 16, VIEW_SIZE_Y + 132 });

    sf::Text text_gameover(font, "", 24);
    text_gameover.setPosition({ 512 - 80, 270 });

    
    // Прямоугольники интерфейса
    sf::RectangleShape rect_selector;
    rect_selector.setOutlineThickness(2);
    rect_selector.setFillColor(sf::Color::Transparent);
    
    sf::RectangleShape rect_progress_border;
rect_progress_border.setOutlineThickness(2);
rect_progress_border.setOutlineColor(sf::Color::White);
rect_progress_border.setPosition({ 1024 - 400 + 8, VIEW_SIZE_Y + 132 });
rect_progress_border.setSize({ 350-16, 32 });
rect_progress_border.setFillColor(sf::Color::Transparent);

sf::RectangleShape rect_progress;
rect_progress.setOutlineThickness(0);
rect_progress.setPosition(rect_progress_border.getPosition());
rect_progress.setFillColor(sf::Color(217, 138, 255));

rect_pblocks.setSize({ 4, 4 });
rect_pblocks.setOutlineThickness(1);
rect_pblocks.setOutlineColor(sf::Color{ 64,64,64 });

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
sf::Music effect_fire("sounds/laser.ogg");
effect_fire.setVolume(0.0f);
effect_fire.setLooping(true);
effect_fire.play();

sf::SoundBuffer effect_start_buffer("sounds/start.ogg");
sf::Sound effect_start(effect_start_buffer);

for (int i = 0; i <= 2; i++) {
    soundbuffers.push_back(std::make_unique<sf::SoundBuffer>("sounds/unicorn_click_"+std::to_string(i)+".ogg"));
    snd_unicorn_clicks.push_back(std::make_unique<sf::Sound>(*soundbuffers.back()));
}
soundbuffers.push_back(std::make_unique<sf::SoundBuffer>("sounds/secret.ogg"));
snd_unicorn_clicks.push_back(std::make_unique<sf::Sound>(*soundbuffers.back()));

ClickerCounter clickcounter(snd_unicorn_clicks.size());

std::optional<sf::Rect<int>> rect_holded = std::nullopt;

addAudioEffect(AudioEffect::Teleport, "sounds/teleport.ogg");
// В файле finish_teleport добавлена пауза в начале, чтобы можно было использовать совместно с эффектом телепортации
addAudioEffect(AudioEffect::FinishTeleport, "sounds/finish_teleport.ogg");
addAudioEffect(AudioEffect::FinishResearch, "sounds/finish_research.ogg");
addAudioEffect(AudioEffect::FinishUpgrade, "sounds/finish_upgrade.ogg");
addAudioEffect(AudioEffect::UnderAttack, "sounds/under_attack.ogg");

addTerrainSprite(Terrain::Ground, "images/terrains/ground.png");
addTerrainSprite(Terrain::Water, "images/terrains/water.png");
addTerrainSprite(Terrain::Forest, "images/terrains/forest.png");
addTerrainSprite(Terrain::Road, "images/terrains/road.png");

addSubTerrainSprite(TerrainSubType::TreeBottom, "images/subterrains/tree_bottom.png");
addSubTerrainSprite(TerrainSubType::TreeBottomLeft, "images/subterrains/tree_bottom_left.png");
addSubTerrainSprite(TerrainSubType::TreeBottomRight, "images/subterrains/tree_bottom_right.png");
addSubTerrainSprite(TerrainSubType::TreeTop, "images/subterrains/tree_top.png");
addSubTerrainSprite(TerrainSubType::TreeTopLeft, "images/subterrains/tree_top_left.png");
addSubTerrainSprite(TerrainSubType::TreeTopRight, "images/subterrains/tree_top_right.png");
addSubTerrainSprite(TerrainSubType::TreeLeft, "images/subterrains/tree_left.png");
addSubTerrainSprite(TerrainSubType::TreeRight, "images/subterrains/tree_right.png");
addSubTerrainSprite(TerrainSubType::WaterTopLeft, "images/subterrains/water_top_left.png");
addSubTerrainSprite(TerrainSubType::WaterTop, "images/subterrains/water_top.png");
addSubTerrainSprite(TerrainSubType::WaterTopRight, "images/subterrains/water_top_right.png");
addSubTerrainSprite(TerrainSubType::WaterLeft, "images/subterrains/water_left.png");
addSubTerrainSprite(TerrainSubType::WaterRight, "images/subterrains/water_right.png");
addSubTerrainSprite(TerrainSubType::WaterBottomLeft, "images/subterrains/water_bottom_left.png");
addSubTerrainSprite(TerrainSubType::WaterBottom, "images/subterrains/water_bottom.png");
addSubTerrainSprite(TerrainSubType::WaterBottomRight, "images/subterrains/water_bottom_right.png");
addSubTerrainSprite(TerrainSubType::RoadHorz, "images/subterrains/road_horz.png");
addSubTerrainSprite(TerrainSubType::RoadVert, "images/subterrains/road_vert.png");
addSubTerrainSprite(TerrainSubType::RoadTopLeft, "images/subterrains/road_top_left.png");
addSubTerrainSprite(TerrainSubType::RoadTopRight, "images/subterrains/road_top_right.png");
addSubTerrainSprite(TerrainSubType::RoadBottomLeft, "images/subterrains/road_bottom_left.png");
addSubTerrainSprite(TerrainSubType::RoadBottomRight, "images/subterrains/road_bottom_right.png");

// Цвета территорий и лазеров
color_terrains[Terrain::Ground] = sf::Color(105, 149, 19);
color_terrains[Terrain::Water] = sf::Color(0, 206, 228);
color_terrains[Terrain::Forest] = sf::Color(25, 103, 39);
color_terrains[Terrain::Road] = sf::Color(228, 218, 171);

color_lasers[LaserType::Harvest] = sf::Color(0, 255, 255);
color_lasers[LaserType::Attack] = sf::Color(255, 0, 0);
color_lasers[LaserType::Heal] = sf::Color(240, 255, 0);
color_lasers[LaserType::Detox] = sf::Color(160, 56, 255);

sfge::Animation laser_apply("images/laser_apply.png", 30, 34, 12, 12);
laser_apply.setOrigin({ 15,17 });
laser_apply.play();

sfge::Animation aura("images/aura_default.png", 86, 80, 12, 12);
aura.setOrigin({ 43, 40 });
aura.play();

anim_lasers[LaserType::Harvest] = &aura;
anim_lasers[LaserType::Attack] = &laser_apply;
anim_lasers[LaserType::Heal] = &aura;
anim_lasers[LaserType::Detox] = &laser_apply;

sfge::Animation teleportation("images/teleportation.png", 96, 96, 9, 9);
teleportation.setOrigin({ 48, 48 });

const sf::Texture texture_laz("images/laser.png");
sf::Sprite spr_laz(texture_laz);
spr_laz.setOrigin({ 0,3 });

sf::Shader shader_gray;
sf::Shader shader_bright;
sf::Shader shader_attack;

shader_gray.loadFromFile("shaders/gray.frag", sf::Shader::Type::Fragment);
shader_bright.loadFromFile("shaders/bright.frag", sf::Shader::Type::Fragment);
shader_attack.loadFromFile("shaders/attack.frag", sf::Shader::Type::Fragment);

shader_gray.setUniform("texture", sf::Shader::CurrentTexture);
shader_bright.setUniform("texture", sf::Shader::CurrentTexture);
shader_attack.setUniform("texture", sf::Shader::CurrentTexture);

float progress;
bool modeendgame = false;
scene = Scenex::Menu;

if (std::filesystem::exists(exedir + "/developer.json"))
    game.loadDeveloperConfig(exedir + "/developer.json");

// Крутим цикл игры
while (window.isOpen())
{
    auto mousePos = sf::Mouse::getPosition(window);

    sf::Sprite* cursor = &cursor_def;

    globalt += dt;

    // Обработка сцены меню
    if (scene == Scenex::Menu) {
        // Дублирование с определением в клике (с учетом доп. кнопки выхода + 1)
        textback.setSize({ 240, 40 });
        for (int i = 0; i < LEVEL_COUNT + 1; i++) {
            textback.setPosition({ 512 - 120, (float)(250 + 64 * i) });
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
                if (keyPressed->scancode == sf::Keyboard::Scancode::M) switchSound();
            };
            if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonReleased>())
            {
                textback.setSize({ 240, 40 });
                for (int i = 0; i < LEVEL_COUNT; i++) {
                    textback.setPosition({ 512 - 100, (float)(250 + 64 * i) });
                    if (textback.getGlobalBounds().contains({ (float)mousePos.x, (float)mousePos.y })) {
                        // Для выбора игры - переходим на сцену задания и загружаем игру
                        scene = Scenex::Task;
                        modeendgame = false;
                        loadGame(i);
                    }
                }
                textback.setPosition({ 512 - 100, (float)(250 + 64 * LEVEL_COUNT) });
                if (textback.getGlobalBounds().contains({ (float)mousePos.x, (float)mousePos.y })) window.close();
            }
        }
    }
    // Для сцены задания
    if (scene == Scenex::Task) {
    while (const std::optional event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>()) window.close();
        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
        {
            // Переход к игре по нажатию на кнопку Esc
            if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)	scene = Scenex::Menu;
        };
        if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonReleased>())
        {
            // Переход к игре по нажатию на кнопку ОК
            textback.setPosition({ 512 - 40, 400 });
            textback.setSize({ 80, 30 });
            if (textback.getGlobalBounds().contains({ (float)mousePos.x, (float)mousePos.y })) scene = Scenex::Game;
        }
    }
    }
    // Далее строго сцена игры
    if (scene == Scenex::Game) {
    // Дублирование с определением при нажатии
    if (mousePos.y < VIEW_SIZE_Y) {
        window.setView(view);
        sf::Vector2f worldpos = window.mapPixelToCoords(mousePos);
        window.setView(window.getDefaultView());

        if (auto uid = game.findUnitAt(worldpos.x, worldpos.y))
            if (!game.isFog(game.getUnitByUID(*uid).getXY().x, game.getUnitByUID(*uid).getXY().y))
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
                scene = Scenex::Menu;
            }
            if (keyPressed->scancode == sf::Keyboard::Scancode::M) switchSound();
            if (keyPressed->scancode == sf::Keyboard::Scancode::Space)
                if (game.getLastEventPos()) {
                    view.setCenter(*game.getLastEventPos());
                    fixCameraPosition();
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
                if (textback.getGlobalBounds().contains({ (float)mousePos.x, (float)mousePos.y })) scene = Scenex::Menu;
            }
        }
        else {
            if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>())
            {
                // Зона игры
                if (mousePressed->position.y < VIEW_SIZE_Y) {
                    window.setView(view);
                    sf::Vector2f worldpos = window.mapPixelToCoords(mousePressed->position);
                    window.setView(window.getDefaultView());

                    // Выделение юнита
                    if (mousePressed->button == sf::Mouse::Button::Left)
                    {
                        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift))
                            rect_holded = sf::IntRect(mousePressed->position, sf::Vector2i(1, 1));

                        if (auto uid = game.findUnitAt(worldpos.x, worldpos.y))
                            if (!game.isFog(game.getUnitByUID(*uid).getXY().x, game.getUnitByUID(*uid).getXY().y)) {
                                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)) {
                                    if (selector.isNoSelected())
                                        selector.selectOneUnit(*uid);
                                    else {
                                        if (game.getUnitByUID(*uid).isComponent<ComponentUnicorn>()) {
                                            bool allunicorn = true;
                                            for (int selid : selector.getSelectedUnits())
                                                allunicorn = allunicorn && game.getUnitByUID(selector.getSelectedUID()).isComponent<ComponentUnicorn>();
                                            if (allunicorn) selector.invertUnit(*uid);
                                        }
                                    }
                                }
                                else {
                                    selector.selectOneUnit(*uid);
                                    if (game.getUnitByUID(selector.getSelectedUID()).isComponent<ComponentUnicorn>())
                                        snd_unicorn_clicks[clickcounter.getNextSoundIdx(selector.getSelectedUID())]->play();
                                }
                            }
                    }

                    // Команда движения юнита
                    if (mousePressed->button == sf::Mouse::Button::Right)
                    {
                        for (int uid: selector.getSelectedUnits())
                            if (game.getUnitByUID(uid).isComponent<ComponentUnicorn>()) {
                                game.setTargetToUnit(uid, worldpos.x / BLOCKW, worldpos.y / BLOCKH);
                                if (((started_galop_uid != uid)||(effect_start.getStatus() != sf::SoundSource::Status::Playing))) {
                                    effect_start.play();
                                    started_galop_uid = uid;
                                }
                            }
                    }
                }
                else
                // Зона карты
                if (minimap.isXYonMap(mousePressed->position)) {
                    view.setCenter(minimap.getWorldPosByMapPos(mousePressed->position.x, mousePressed->position.y));
                    fixCameraPosition();
                    mouseholdedonmap = true;
                }
                // Зона действий
                else {
                    if (selector.isSelectedOne()) {
                        std::string msgcode = "";
                        // Разрешаем действия только если юнит не работает над действием в данный момент
                        if (!game.getUnitByUID(selector.getSelectedUID()).isWorkingTask()) {
                            auto actions = game.getUnitByUID(selector.getSelectedUID()).getActions();
                            for (int i = 0; i < actions.size(); i++) {
                                spr_but_action.setPosition(getActionButtonPos(i));
                                if (spr_but_action.getGlobalBounds().contains({ (float)mousePressed->position.x, (float)mousePressed->position.y })) {
                                    if (!game.getUnitByUID(selector.getSelectedUID()).canSendAction(actions[i], &msgcode)) {
                                        text_action.setString(texts.getSfmlStr(msgcode));
                                        counter_errmsg.upset(1.0f);
                                    }
                                    else
                                        game.sendUnitAction(selector.getSelectedUID(), actions[i]);
                                }
                            }
                        }
                        else {
                            // Действие отмены
                            if (undo.getGlobalBounds().contains({ (float)mousePressed->position.x, (float)mousePressed->position.y })) {
                                game.cancelUnitWorkingAction(selector.getSelectedUID());
                            }
                        }
                    }
                }
            }
            // Применение рамки только по отжатию
            if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonReleased>()) {
                if (rect_holded) {
                    window.setView(view);
                    sf::Vector2f worldpos_1 = window.mapPixelToCoords((*rect_holded).position);
                    sf::Vector2f worldpos_2 = window.mapPixelToCoords((*rect_holded).position + (*rect_holded).size);
                    window.setView(window.getDefaultView());

                    auto uids = game.findUnitsInRect(std::min(worldpos_1.x, worldpos_2.x), std::min(worldpos_1.y, worldpos_2.y),
                        std::max(worldpos_1.x, worldpos_2.x), std::max(worldpos_1.y, worldpos_2.y));
                    if (uids.size() > 0) {
                        // Выделяем всех единорогов в списке
                        selector.unSelectAll();
                        for (int uid : uids)
                            if (game.getUnitByUID(uid).isComponent<ComponentUnicorn>())
                                selector.invertUnit(uid);
                        // Если никто не выделился, то пробуем тогда выделить одного не-единорога
                        if (selector.isNoSelected())
                            for (int uid : uids)
                                if (!game.getUnitByUID(uid).isComponent<ComponentUnicorn>()) {
                                    selector.invertUnit(uid);
                                    break;
                                }
                    }
                    rect_holded = std::nullopt;
                }
            }
        }
    }
    
    if (!modeendgame) {
        // Прокрутка карты
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left)) {
            view.move({ -SCROLLSPEED * BLOCKW * getScale05per20() * dt, 0 });
            fixCameraPosition();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right)) {
            view.move({ +SCROLLSPEED * BLOCKW * getScale05per20() * dt, 0 });
            fixCameraPosition();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up)) {
            view.move({ 0, -SCROLLSPEED * BLOCKH * getScale05per20() * dt });
            fixCameraPosition();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Down)) {
            view.move({ 0, SCROLLSPEED * BLOCKH * getScale05per20() * dt });
            fixCameraPosition();
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)&& mouseholdedonmap) {
            view.setCenter(minimap.getWorldPosByMapPos(mousePos.x, mousePos.y));
            fixCameraPosition();
        }
        else
            mouseholdedonmap = false;

        laser_apply.update(dt);
        aura.update(dt);
        game.update(dt);
        for (int uid: selector.getSelectedUnits())
            if (!game.isUnitExist(uid)) selector.unSelectUnit(uid);

        updateMiniMap(game);

        if (rect_holded) {
            (*rect_holded).size.x = mousePos.x - (*rect_holded).position.x;
            (*rect_holded).size.y = mousePos.y - (*rect_holded).position.y;
        }
    }

    counter_errmsg.update(dt);
    counter_endgame.update(dt);

    if (effect_start.getStatus() == sf::SoundSource::Status::Playing)
        if (game.isUnitExist(started_galop_uid))
            if (!game.getUnitByUID(started_galop_uid).isTargeted()) effect_start.stop();

    // Добавление эффекта телепортации
    if (auto new_effect = game.getOnceTeleportationEffect()) {
        current_teleportation_effect = teleportation;
        (*current_teleportation_effect).setPosition(*new_effect);
        (*current_teleportation_effect).playOneTime();
    }

    // Обработка эффекта телепортации
    if (current_teleportation_effect) {
        (*current_teleportation_effect).update(dt);
        if (!(*current_teleportation_effect).isPlayed()) current_teleportation_effect = std::nullopt;
    }

    // Обработка разных эффектов
    for (auto effect : game.getOnceAudioEffects())
        snd_audioeffects[effect]->play();

    if (counter_endgame.onceReachNol()) {
        // Инициализация механизма завершения уровня - отключение звуковых эффектов
        modeendgame = true;
        effect_fire.setVolume(0.0f);
        effect_start.stop();
    }

    // Обновление камеры если нужно
    if (auto newvp = game.getOnceNewViewPoint())
        view.setCenter({ (float)(*newvp).x * BLOCKW, (float)(*newvp).y * BLOCKH });

    fogbuilder.updateByGame(game);

    shader_attack.setUniform("stage", (float)(0.25f+0.25f*sin(4.0f*M_PI*globalt)));

    if (!modeendgame)
        if (game.isGameOver())
            if (!counter_endgame.isActive()) counter_endgame.upset(2.0f);

        }
        window.clear();

        // Для сцены меню - вывод меню и фона
        if (scene == Scenex::Menu) {
            window.draw(spr_intro);
            window.draw(spr_title);

            window.draw(text_help);
            window.draw(text_version);

            textback.setSize({ 240, 40 });
            for (int i = 0; i < LEVEL_COUNT; i++) {
                textback.setPosition({ 512-120, (float)(250 + 64 * i) });
                window.draw(textback);

                text_info.setString(texts.getSfmlStr("Name_Level_" + std::to_string(i)));
                text_info.setPosition({ 512 - text_info.getLocalBounds().size.x/2, (float)(250 + 64 * i) + 8 });
                window.draw(text_info);
            }
            textback.setPosition({ 512 - 120, (float)(250 + 64 * LEVEL_COUNT) });
            window.draw(textback);

            text_info.setString(texts.getSfmlStr("Text_Quit"));
            text_info.setPosition({ 512 - text_info.getLocalBounds().size.x / 2, (float)(250 + 64 * LEVEL_COUNT) + 8 });
            window.draw(text_info);
        }
        // Для сцены задачи и игры - вывод территорий
        if ((scene == Scenex::Task) || (scene == Scenex::Game)) {
            window.setView(view);

                for (int i = 0; i < game.getWidth(); i++)
                    for (int j = 0; j < game.getHeight(); j++)
                        if (!game.isFog(i, j)) {
                            if (auto treeblock = stbuilder.getTerrainSubType(i, j)) {
                                // Этот трюк нужен, чтобы сначала вывелись территории и фрагменты леса нижние, а потом - верхние, закрывающие пони
                                if (!((*treeblock == TerrainSubType::TreeTop) || (*treeblock == TerrainSubType::TreeTopLeft) || (*treeblock == TerrainSubType::TreeTopRight))) {
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

                // Вывод грибов
                for (int i = 0; i < game.getWidth(); i++)
                    for (int j = 0; j < game.getHeight(); j++)
                        if (!game.isFog(i, j)) {
                            auto & mset = game.getMushrooms(i, j);
                            for (auto & m : mset) {
                                spr_mushrooms[m.spriteid]->setPosition(sf::Vector2f(i* BLOCKW + m.x, j* BLOCKH + m.y ));
                                window.draw(*spr_mushrooms[m.spriteid]);
                            }
                        }

                // Вывод юнитов
                for (int i = 0; i < game.getUnitCount(); i++)
                    if (!game.isFog(game.getUnit(i).getXY().x, game.getUnit(i).getXY().y))
                        if (spr_units.count(game.getUnit(i).getCode()) > 0) {
                            bool movleft = game.getUnit(i).getLastMoving() == Moving::Left;
                            spr_units[game.getUnit(i).getCode()]->setPosition(game.getUnit(i).getView());
                            spr_units[game.getUnit(i).getCode()]->setScale({ movleft ? -1.0f : 1.0f,1 });
                            if (game.isUnitUnderAttack(game.getUnit(i).getUID()))
                                window.draw(*spr_units[game.getUnit(i).getCode()], &shader_attack);
                            else
                                window.draw(*spr_units[game.getUnit(i).getCode()]);

                            if (game.getUnit(i).isComponent<ComponentUnicorn>()) {
                                int marker_x = game.getUnit(i).getView().x + (movleft ? 1 : -1)*(game.getUnit(i).getSizeView().x / 2 - 15);
                                int marker_y = game.getUnit(i).getView().y - game.getUnit(i).getSizeView().y / 2 + 5;
                                for (int j = 0; j < game.getUnit(i).getPostfixes().size(); j++) {
                                    marker.setPosition(sf::Vector2f(marker_x+ (movleft ? 1 : -1)*marker_dx[j],marker_y+ marker_dy[j]));
                                    marker.setColor(components_color[game.getUnit(i).getPostfixes()[j]]);
                                    window.draw(marker);
                                }
                            }
                        }

                // И здесь вывод только верхних фрагментов леса
                for (int i = 0; i < game.getWidth(); i++)
                    for (int j = 0; j < game.getHeight(); j++)
                        if (!game.isFog(i, j)) {
                            if (auto treeblock = stbuilder.getTerrainSubType(i, j)) {
                                if ((*treeblock == TerrainSubType::TreeTop) || (*treeblock == TerrainSubType::TreeTopLeft) || (*treeblock == TerrainSubType::TreeTopRight)) {
                                    spr_trees[*treeblock]->setPosition(sf::Vector2f(i * BLOCKW, j * BLOCKH));
                                    window.draw(*spr_trees[*treeblock]);
                                }
                            }
                        }

            // Полоски здоровья, щита и прогресса выводим после юнитов
                for (int i = 0; i < game.getUnitCount(); i++)
                    if (!game.isFog(game.getUnit(i).getXY().x, game.getUnit(i).getXY().y)) {
                        drawProgressRectsAt(window, game.getUnit(i).getHealthPerMax(), game.getUnit(i).getSizeView().x,
                            game.getUnit(i).getView().x - game.getUnit(i).getSizeView().x / 2.0f,
                            game.getUnit(i).getView().y - game.getUnit(i).getSizeView().y / 2.0f - 8,
                            getColorByHPNorm(game.getUnit(i).getHealthPerMax()));
                        float v = game.getUnit(i).getShieldPerMax();
                        int shift = 14;
                        if (v > 0) {
                            drawProgressRectsAt(window, v, game.getUnit(i).getSizeView().x,
                                game.getUnit(i).getView().x - game.getUnit(i).getSizeView().x / 2.0f,
                                game.getUnit(i).getView().y - game.getUnit(i).getSizeView().y / 2.0f - shift,
                                sf::Color(107, 230, 255));
                            shift += 6;
                        }
                        if (game.getUnit(i).isWorkingTask(&v))
                            drawProgressRectsAt(window, v, game.getUnit(i).getSizeView().x,
                                game.getUnit(i).getView().x - game.getUnit(i).getSizeView().x / 2.0f,
                                game.getUnit(i).getView().y - game.getUnit(i).getSizeView().y / 2.0f - shift,
                                sf::Color(217, 138, 255));
                    }

            // Лазеры выводим после всего
            for (int i = 0; i < game.getLaserCount(); i++)
                drawLaserFromTo(window, spr_laz, game.getLaser(i));

            // Фрагменты тумана в конце
            for (int i = 0; i < game.getWidth(); i++)
                for (int j = 0; j < game.getHeight(); j++)
                    if (!game.isFog(i, j))
                        if (auto sprfog = fogbuilder.getFogSprite(i, j)) {
                            (*sprfog).setPosition(sf::Vector2f(i* BLOCKW, j* BLOCKH));
                            window.draw(*sprfog);
                        }

            for (int uid: selector.getSelectedUnits()) {
                if (game.getUnitByUID(uid).isComponent<ComponentResource>())
                    rect_selector.setOutlineColor(sf::Color::Blue);
                else
                    if (game.getUnitByUID(uid).isComponent<ComponentEnemy>())
                        rect_selector.setOutlineColor(sf::Color::Red);
                    else
                        rect_selector.setOutlineColor(sf::Color::Green);
                rect_selector.setPosition(game.getUnitByUID(uid).getView());
                rect_selector.setSize(game.getUnitByUID(uid).getSizeView());
                rect_selector.setOrigin(game.getUnitByUID(uid).getSizeView() / 2.0f);
                window.draw(rect_selector);
            }

            // Эффект телепортации
            if (current_teleportation_effect) window.draw(*current_teleportation_effect);

            window.setView(window.getDefaultView());
            // Далее рисуем управлющие элементы панели
            window.draw(spr_border);

            minimap.drawTo(&window);

            textback.setPosition({ 6, 6 });
            textback.setSize({ 240, 30 });
            window.draw(textback);

            // Информация по ресурсам и танкам
            text_resource.setString(sfge::SfmlTools::utf2text(texts.getStr("Text_Energy") + " " + std::to_string(game.getEnergy())));
            text_resource.setPosition({ 10, 10 });
            text_resource.setFillColor(sf::Color{ 162, 231, 255 });
            window.draw(text_resource);
            text_resource.setString(sfge::SfmlTools::utf2text(texts.getStr("Text_UnicornCount") + " " + game.getUnicornCountInfo()));
            text_resource.setPosition({ 256 / 2 + 12, 10 });
            text_resource.setFillColor(sf::Color::White);
            window.draw(text_resource);

            if (selector.isSelectedOne()) {
                // Информация по юниту
                // Здесь мы привязываем к позиции text_back без его вывода
                textback.setSize({ 240, 192 });
                textback.setPosition({ 512 - textback.getSize().x/2 - 64, 768 - textback.getSize().y });

                const GameUnit& selunit = game.getUnitByUID(selector.getSelectedUID());
                drawProgressRectsAt(window, selunit.getHealthPerMax(), 48, textback.getPosition().x + 12, textback.getPosition().y + 64,
                    getColorByHPNorm(selunit.getHealthPerMax()));
                if (selunit.getShieldPerMax() > 0.0f)
                    drawProgressRectsAt(window, selunit.getShieldPerMax(), 48, textback.getPosition().x + 12, textback.getPosition().y + 82,
                        sf::Color(107, 230, 255));

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

                text_hp.setString(selunit.getShieldInfo());
                text_hp.setPosition({ textback.getPosition().x + 78, textback.getPosition().y + 72 });
                text_hp.setFillColor(sf::Color(107, 230, 255));
                window.draw(text_hp);

                text_info.setString(texts.getSfmlStrReplacedConsts(selunit.getComponentsInfo()));
                text_info.setPosition({ textback.getPosition().x + 12, textback.getPosition().y + 92 });
                window.draw(text_info);
                
                // Действия юнита
                // Здесь мы привязываем к позиции text_back без его вывода
                textback.setSize({ 400, 192 });
                textback.setPosition({ 1024 - textback.getSize().x, 768 - textback.getSize().y });
               
                std::string current_action_code;
                if (selunit.isWorkingTask(&progress, &current_action_code)) {
                    window.draw(rect_progress_border);
                    rect_progress.setSize({ (350-16) * progress, 32 });
                    window.draw(rect_progress);
                    text_progress.setString(std::to_string((int)(100 * progress)) + "%");
                    window.draw(text_progress);
                    spr_actions[current_action_code]->setPosition({ (float)(textback.getPosition().x + 200 - 32), textback.getPosition().y + 8 });
                    window.draw(*spr_actions[current_action_code]);
                    if (undo.getGlobalBounds().contains({ (float)mousePos.x,(float)mousePos.y })) {
                        window.draw(undo, &shader_bright);
                        cursor = &cursor_my;
                    }
                    else 
                        window.draw(undo);
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
                            text_action.setString(sfge::SfmlTools::utf2text(
                                texts.getStr("Action_" + actions[i].caption) + "\n" + texts.getStr("Text_Energy") + " " + std::to_string(actions[i].energy)));
                            text_action.setFillColor(sf::Color::White);
                            window.draw(text_action);
                        }
                    }
                }
            }
            if (selector.isSelectedMulti()) {
                const int UNIT_IN_ROW = 10;
                const int UNIT_STEP_X = 64;
                const int UNIT_STEP_Y = 84;
                // Здесь мы привязываем к позиции text_back без его вывода
                textback.setSize({ 240, 182 });
                textback.setPosition({ 512 - textback.getSize().x / 2 - 64, 768 - textback.getSize().y });
                for (int i = 0; i < selector.getSelectedUnits().size(); i++) {
                    const GameUnit& selunit = game.getUnitByUID(selector.getSelectedUnits()[i]);
                    drawProgressRectsAt(window, selunit.getHealthPerMax(), 48,
                        textback.getPosition().x + 12 + (i % UNIT_IN_ROW) * UNIT_STEP_X, textback.getPosition().y + 64 + (i / UNIT_IN_ROW) * UNIT_STEP_Y,
                        getColorByHPNorm(selunit.getHealthPerMax()));
                    if (selunit.getShieldPerMax() > 0.0f)
                        drawProgressRectsAt(window, selunit.getShieldPerMax(), 48,
                            textback.getPosition().x + 12 + (i % UNIT_IN_ROW) * UNIT_STEP_X, textback.getPosition().y + 70 + (i / UNIT_IN_ROW) * UNIT_STEP_Y,
                            sf::Color(107, 230, 255));
                    if (spr_icons.count(selunit.getCode()) > 0) {
                        spr_icons[selunit.getCode()]->setPosition({ textback.getPosition().x + 12 + 48 / 2 + (i % UNIT_IN_ROW) * UNIT_STEP_X,
                            textback.getPosition().y + 34 + (i / UNIT_IN_ROW) * UNIT_STEP_Y });
                        window.draw(*spr_icons[selunit.getCode()]);
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

                text_timer.setString(sfge::SfmlTools::utf2text(texts.getStr("Text_Timer") + " " + *stimer));
                text_timer.setPosition({ 1024 - 144 + 10, 8 });
                window.draw(text_timer);
            }

            if (rect_holded) {
                rect_selector.setOutlineColor(sf::Color::Green);
                rect_selector.setPosition(sf::Vector2f((*rect_holded).position.x, (*rect_holded).position.y));
                rect_selector.setSize(sf::Vector2f((*rect_holded).size.x, (*rect_holded).size.y));
                rect_selector.setOrigin(sf::Vector2f(0,0));
                window.draw(rect_selector);
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
            if (scene == Scenex::Task) {
                textback.setPosition({512 - 200, 286 });
                textback.setSize({ 400, 150 });
                window.draw(textback);

                text_task.setString(texts.getSfmlStr("Text_Task")+"\n"+sfge::SfmlTools::utf2text(game.getTaskText()));
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
    */

    return 0;
}
