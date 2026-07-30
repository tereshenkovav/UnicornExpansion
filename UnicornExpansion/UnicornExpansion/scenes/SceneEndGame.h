#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "SfmlGameEngine/Scene.h"
#include "SfmlGameEngine/Button.h"
#include "Game.h"

class SceneEndGame: public sfge::Scene
{
private:
    sf::RectangleShape textback;
    sfge::UniqueText text_title;
    std::vector<sfge::UniqueButton> buttons;
    LevelCode levelcode;
    bool iswin;
    void goNextLevel();
    void goOutOfLevel();
public:
    SceneEndGame(const Game& game, LevelCode levelcode);
    virtual void Render(sf::RenderTarget& rendertarget) override;
    virtual void Update(float dt, const sf::Vector2i& mousePos, const std::vector<sf::Event>& events) override;
    virtual void Init() override;
};
