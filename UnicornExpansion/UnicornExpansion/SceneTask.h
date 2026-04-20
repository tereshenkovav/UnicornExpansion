#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "SfmlGameEngine/Scene.h"
#include "SfmlGameEngine/Button.h"
#include "Game.h"

class SceneTask: public sfge::Scene
{
private:
    sf::RectangleShape textback;
    std::unique_ptr<sf::Text> text_task;
    std::unique_ptr<sf::Text> text_title;
    std::unique_ptr<sfge::Button> butok;
    std::string task;
public:
    SceneTask(const Game & game);
    virtual void Render(sf::RenderTarget& rendertarget) override;
    virtual void Update(float dt, const sf::Vector2i& mousePos, const std::vector<sf::Event>& events) override;
    virtual void Init() override;
};
