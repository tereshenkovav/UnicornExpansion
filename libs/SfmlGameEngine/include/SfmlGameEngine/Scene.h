#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

namespace sfge {

class Engine;

class Scene
{
protected:
    Engine* engine;
public:
    virtual void Render(sf::RenderTarget& rendertarget);
    virtual void Update(float dt, const sf::Vector2i& mousePos, const std::vector<sf::Event>& events);
    virtual void Init();
    virtual void UnInit();

    void setEngine(Engine* engine);
};

};
