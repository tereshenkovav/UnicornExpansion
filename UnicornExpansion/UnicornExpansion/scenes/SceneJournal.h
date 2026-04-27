#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "SfmlGameEngine/Scene.h"
#include "SfmlGameEngine/Button.h"
#include "Game.h"

class SceneJournal: public sfge::Scene
{
private:
    std::string msg;
    sf::RectangleShape textback;
    sfge::UniqueText text_msgs;
    sfge::UniqueText text_title;
    sfge::UniqueButton butok;
    std::vector<GameTask> tasks;
public:
    SceneJournal(const Game & game);
    virtual void Render(sf::RenderTarget& rendertarget) override;
    virtual void Update(float dt, const sf::Vector2i& mousePos, const std::vector<sf::Event>& events) override;
    virtual void Init() override;
};
