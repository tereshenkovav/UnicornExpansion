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
    std::unique_ptr<sf::Text> text_title;
    std::vector<std::unique_ptr<sfge::Button>> buttons;
    std::unique_ptr<sfge::Button> butcontinue;
    std::unique_ptr<sfge::Button> butmenu;
    int leveln;
    bool iswin;
public:
    SceneEndGame(const Game& game, int leveln);
    virtual void Render(sf::RenderTarget& rendertarget) override;
    virtual void Update(float dt, const sf::Vector2i& mousePos, const std::vector<sf::Event>& events) override;
    virtual void Init() override;
    virtual void UnInit() override;
};
