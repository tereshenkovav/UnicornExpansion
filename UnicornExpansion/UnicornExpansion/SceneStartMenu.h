#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "SfmlGameEngine/Scene.h"
#include "SfmlGameEngine/Button.h"

class SceneStartMenu: public sfge::Scene
{
private:
    std::unique_ptr<sf::Text> text_caption;
    std::unique_ptr<sf::Text> text_version;
    std::unique_ptr<sf::Sound> snd_click;
    std::vector<std::unique_ptr<sfge::Button>> buttons;
public:
    virtual void Render(sf::RenderTarget& rendertarget) override;
    virtual void Update(float dt, const sf::Vector2i& mousePos, const std::vector<sf::Event>& events) override;
    virtual void Init() override;
};
