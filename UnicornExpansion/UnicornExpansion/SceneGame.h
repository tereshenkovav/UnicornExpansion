#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "SfmlGameEngine/Scene.h"

class SceneGame: public sfge::Scene
{
private:
public:
    virtual void Render(sf::RenderTarget& rendertarget) override;
    virtual void Update(float dt, const sf::Vector2i& mousePos, const std::vector<sf::Event>& events) override;
    virtual void Init() override;
    virtual void UnInit() override;
};
