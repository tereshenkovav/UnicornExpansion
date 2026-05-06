#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "SfmlGameEngine/Scene.h"

class SceneMainMenu: public sfge::Scene
{
private:
    sfge::UniqueSprite spr_intro;
    sfge::UniqueSprite spr_title;
    std::optional<std::string> company;
public:
    SceneMainMenu();
    SceneMainMenu(const std::string & company);
    virtual void Render(sf::RenderTarget& rendertarget) override;
    virtual void Update(float dt, const sf::Vector2i& mousePos, const std::vector<sf::Event>& events) override;
    virtual void Init() override;
};
