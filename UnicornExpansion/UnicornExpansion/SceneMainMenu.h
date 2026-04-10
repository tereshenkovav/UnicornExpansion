#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "SfmlGameEngine/Scene.h"
#include "Texts.h"

class SceneMainMenu: public sfge::Scene
{
private:
    sf::RectangleShape textback;
    std::unique_ptr<sf::Sprite> spr_intro;
    std::unique_ptr<sf::Sprite> spr_title;
    std::unique_ptr<sf::Text> text_help;
    std::unique_ptr<sf::Text> text_version;
    std::unique_ptr<sf::Text> text_info;
    std::unique_ptr<sf::Sprite> cursor;
    sf::Vector2i mousePos;    
    Texts texts;
public:
    virtual void Render(sf::RenderTarget& rendertarget) override;
    virtual void Update(float dt, const sf::Vector2i& mousePos, const std::vector<sf::Event>& events) override;
    virtual void Init() override;
    virtual void UnInit() override;
};
