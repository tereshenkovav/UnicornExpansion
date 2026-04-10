#include "SfmlGameEngine/Engine.h"
#include "SfmlGameEngine/SFMLTools.h"

namespace sfge {

Engine::Engine(unsigned int width, unsigned int height)
{
    this->width = width;
    this->height = height;
    sf::ContextSettings settings;
    settings.antiAliasingLevel = 8;

    // Создание окна
    window = std::make_unique<sf::RenderWindow>(sf::VideoMode({ width, height }), "", sf::Style::Close, sf::State::Windowed, settings);
    window->setMouseCursorVisible(false);
    window->setVerticalSyncEnabled(true);
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

void Engine::setCaption(const std::string& str)
{
    window->setTitle(SfmlTools::utf2text(str));
}

void Engine::setIcon(const std::string& filename)
{
    window->setIcon(sf::Image(filename));
}

void Engine::doClose()
{
    closed = true;
}

void Engine::SwitchToScene(std::shared_ptr<Scene> scene)
{
    nextscene = scene;
}

void Engine::Run(std::shared_ptr<Scene> scene)
{    
    std::shared_ptr<Scene> tekscene = scene;
    tekscene->setEngine(this);
    tekscene->Init();
    
    nextscene = std::shared_ptr<Scene>();

	sf::Clock clock;

    std::vector<sf::Event> events;
    closed = false;
    // Крутим цикл игры
    while (window->isOpen())
    {
        auto mousepos = sf::Mouse::getPosition(*window);

        float dt = clock.getElapsedTime().asSeconds();
        clock.restart();

        events.clear();
        // Получаем все события от окна
        while (const std::optional event = window->pollEvent())
            if (event->is<sf::Event::Closed>()) window->close(); else events.push_back(*event);

        tekscene->Update(dt, mousepos, events);
        
        window->clear();
        tekscene->Render(*window);
        window->display();

        if (nextscene) {
            tekscene->UnInit();
            tekscene = std::move(nextscene);
            tekscene->setEngine(this);
            tekscene->Init();
        }

        // Последняя строка в цикле
        if (closed) window->close();
    }
    tekscene->UnInit();
}

}
