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

void Engine::Run(std::shared_ptr<Scene> scene)
{    
    std::shared_ptr<Scene> tekscene = scene;
    tekscene->setEngine(this);
    tekscene->Init();
    
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

        // Последняя строка в цикле
        if (closed) window->close();
    }
    tekscene->UnInit();
}

}