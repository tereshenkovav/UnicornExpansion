#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "SfmlGameEngine/Scene.h"

class SceneMainMenu: public sfge::Scene
{
private:
    std::unique_ptr<sf::Sprite> spr_intro;
    std::unique_ptr<sf::Sprite> spr_title;
public:
    virtual void Render(sf::RenderTarget& rendertarget) override;
    virtual void Update(float dt, const sf::Vector2i& mousePos, const std::vector<sf::Event>& events) override;
    virtual void Init() override;
};
