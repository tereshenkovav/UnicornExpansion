#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "SfmlGameEngine/Scene.h"
#include "SfmlGameEngine/Button.h"
#include "Game.h"

class SceneGameMenu: public sfge::Scene
{
private:
    sf::RectangleShape textback;
    std::vector<sfge::UniqueButton> buttons;
    int leveln;
    Game* game;
public:
    SceneGameMenu(Game * game, int leveln);
    virtual void Render(sf::RenderTarget& rendertarget) override;
    virtual void Update(float dt, const sf::Vector2i& mousePos, const std::vector<sf::Event>& events) override;
    virtual void Init() override;
};
