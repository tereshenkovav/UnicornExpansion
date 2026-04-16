#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "SfmlGameEngine/Scene.h"
#include "SfmlGameEngine/Button.h"

class SceneMainMenu: public sfge::Scene
{
private:
    std::unique_ptr<sf::Sprite> spr_intro;
    std::unique_ptr<sf::Sprite> spr_title;
    std::unique_ptr<sf::Text> text_help;
    std::unique_ptr<sf::Text> text_version;
    std::unique_ptr<sf::Sound> snd_click;
    std::vector<std::unique_ptr<sfge::Button>> buttons;
public:
    virtual void Render(sf::RenderTarget& rendertarget) override;
    virtual void Update(float dt, const sf::Vector2i& mousePos, const std::vector<sf::Event>& events) override;
    virtual void Init() override;
    virtual void UnInit() override;
};
