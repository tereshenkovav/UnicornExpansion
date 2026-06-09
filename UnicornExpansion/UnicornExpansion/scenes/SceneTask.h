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
    sfge::UniqueText text_task;
    sfge::UniqueText text_title;
    sfge::UniqueButton butok;
    std::vector<GameTask> tasks;
    std::string task ;
public:
    SceneTask(const Game & game);
    virtual void Render(sf::RenderTarget& rendertarget) override;
    virtual void Update(float dt, const sf::Vector2i& mousePos, const std::vector<sf::Event>& events) override;
    virtual void Init() override;
};
