#include "SfmlGameEngine/Engine.h"
#include "SfmlGameEngine/SFMLTools.h"

namespace sfge {

Engine::Engine(unsigned int width, unsigned int height)
{
    this->width = width;
    this->height = height;
    this->profile = std::make_shared<Profile>();
}

void Engine::setStopUpdatingForLostFocus(bool value)
{
    this->stopupdatingforlostfocus = value;
}

void Engine::loadDefaultFont(const std::string& filename)
{
    defaultfont = std::make_shared<sf::Font>(filename);
}

std::shared_ptr<sf::Font> Engine::getDefaultFont() const
{
    if (!defaultfont) throw sf::Exception("Not set default font for Engine");
    return defaultfont;
}

void Engine::loadDefaultCursor(const std::string& filename)
{
    textures.push_back(std::make_unique<sf::Texture>(filename));
    defaultcursor = std::make_shared<sf::Sprite>(*textures.back());
}

void Engine::addCursor(int code, const std::string& filename)
{
    textures.push_back(std::make_unique<sf::Texture>(filename));
    cursors[code]=std::make_shared<sf::Sprite>(*textures.back());
}

void Engine::setExeDir(const std::string& exedir) {
    this->exedir = exedir;
}

std::string Engine::getExeDir() const {
    return exedir;
}

void Engine::setCursor(int code)
{
    if (cursors.count(code) == 0) throw sf::Exception("Not found cursor with code: " + std::to_string(code));
    currentcursor = cursors[code];
}

void Engine::setCaption(const std::string& str)
{
    windowtitlestr = str;
    if (window) window->setTitle(SfmlTools::utf2text(windowtitlestr));
}

void Engine::setIcon(const std::string& filename)
{
    windowicon = sf::Image(filename);
    if (window) window->setIcon(*windowicon);
}

void Engine::doClose()
{
    signal_closed = true;
}

void Engine::doExitScene() {
    signal_exitscene = true;
}

void Engine::SwitchToScene(std::shared_ptr<Scene> scene)
{
    nextscene = scene;
}

void Engine::AddOverScene(std::shared_ptr<Scene> scene)
{
    overscene = scene;
}

void Engine::ReplaceOverScene(std::shared_ptr<Scene> scene)
{
    replacedoverscene = scene;
}

void Engine::loadTexts(const std::string& filename)
{
    texts.loadFromFile(filename);
}

const Texts& Engine::getTexts() const
{
    return texts;
}

void Engine::loadColors(const std::string& filename)
{
    colors.loadFromFile(filename);
}

const Colors& Engine::getColors() const
{
    return colors;
}

sf::Vector2f Engine::getWorldPosByView(const sf::View& view, sf::Vector2i pos)
{
    window->setView(view);
    sf::Vector2f worldpos = window->mapPixelToCoords(pos);
    window->setView(window->getDefaultView());
    return worldpos;
}

int Engine::getFactFPS() const {
    return factfps;
}

float Engine::getAllTime() const {
    return alltime;
}

std::shared_ptr<Profile> Engine::getProfile() const {
    return profile;
}

void Engine::updateByProfile() {
    if (window) window->setVerticalSyncEnabled(profile->isVSync());
    sf::Listener::setGlobalVolume(profile->isSoundOn() ? 100.0f : 0.0f);
    if (isfullscr != profile->isFullScreen()) signal_rebuild = true;
}

void Engine::createWindow() {
    sf::ContextSettings settings;
    settings.antiAliasingLevel = 8;

    // Создание окна
    window = std::make_unique<sf::RenderWindow>(sf::VideoMode({ width, height }), "", sf::Style::Close,
        profile->isFullScreen() ? sf::State::Fullscreen : sf::State::Windowed, settings);
    updateByProfile();
    window->setTitle(SfmlTools::utf2text(windowtitlestr));
    if (windowicon) window->setIcon(*windowicon);

    window->setMouseCursorVisible(false);
    isfullscr = profile->isFullScreen();
}

void Engine::Run(std::shared_ptr<Scene> scene)
{
    createWindow();

    std::vector<std::shared_ptr<Scene>> scenes = { scene };
    scenes.back()->setEngine(this);
    scenes.back()->Init();

    signal_closed = false;
    signal_exitscene = false;
    signal_rebuild = false;

    alltime = 0;
    sf::Clock clock;
    std::vector<sf::Event> events;
    float timefps = 0.0f;
    int calcfps = 0;

    // Крутим цикл игры
    while (window->isOpen())
    {
        auto mousepos = sf::Mouse::getPosition(*window);

        float dt = clock.getElapsedTime().asSeconds();
        clock.restart();
        alltime += dt;
        
        // Вычисление фактического FPS
        timefps += dt;
        calcfps++;
        if (timefps >= 1.0f) {
            factfps = calcfps; timefps = 0.0f; calcfps = 0;
        }

        events.clear();
        // Получаем все события от окна
        while (const std::optional event = window->pollEvent())
            if (event->is<sf::Event::Closed>()) window->close(); else events.push_back(*event);

        // Ставим курсор как пустой по умолчанию, и дополняем, если он был установлен в default
        currentcursor = std::weak_ptr<sf::Sprite>();
        if (defaultcursor) currentcursor = defaultcursor;

        // Если окно активно или не установлен флаг остановки обновлений при потере фокуса
        if ((window->hasFocus())||(!stopupdatingforlostfocus))
            // Обновление сцены - только верхний уровень
            if (!scenes.empty()) scenes.back()->Update(dt, mousepos, events);

        // Рендер сцены - снизу вверх все сцены
        window->clear();
        for (auto& scene : scenes) scene->Render(*window);
        // Курсор в конце сцены        
        if (auto cursor = currentcursor.lock()) {
            int delta = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) ? 4 : 0;
            cursor->setPosition({ (float)mousepos.x + delta,(float)mousepos.y + delta });
            window->draw(*cursor);
        }
        window->display();

        // Обработка сигналов на переключение сцены, добавление сверхсцены, замену сверхсцены и закрытие
        if (nextscene) {
            for (auto& scene : scenes) scene->UnInit();
            scenes.clear();

            scenes.push_back(std::move(nextscene));
            scenes.back()->setEngine(this);
            scenes.back()->Init();
        }

        if (overscene) {
            scenes.push_back(std::move(overscene));
            scenes.back()->setEngine(this);
            scenes.back()->Init();
        }

        if (replacedoverscene) {
            if (scenes.size() > 1) {
                scenes.back()->UnInit();
                scenes.pop_back();
                scenes.push_back(std::move(replacedoverscene));
                scenes.back()->setEngine(this);
                scenes.back()->Init();
            }
            else
                replacedoverscene.reset();
        }

        if (signal_exitscene) {
            signal_exitscene = false;
            if (!scenes.empty()) {
                scenes.back()->UnInit();
                scenes.pop_back();
                if (scenes.empty()) window->close();
            }
        }

        if (signal_rebuild) {
            signal_rebuild = false;
            window->close();
            window.reset();
            createWindow();
        }

        // Последняя строка в цикле
        if (signal_closed) window->close();
    }
    // Очистка всех сцен
    for (auto& scene : scenes) scene->UnInit();
    // Избыточно в конце метода для локального объекта, но пусть будет для ясности
    scenes.clear();
}

}
