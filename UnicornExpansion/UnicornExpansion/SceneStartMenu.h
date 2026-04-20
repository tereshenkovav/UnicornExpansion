#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "SfmlGameEngine/Scene.h"
#include "SfmlGameEngine/Button.h"

class SceneStartMenu: public sfge::Scene
{
private:
    sfge::UniqueText text_caption;
    sfge::UniqueText text_version;
    sfge::UniqueSound snd_click;
    std::vector<sfge::UniqueButton> buttons;
public:
    virtual void Render(sf::RenderTarget& rendertarget) override;
    virtual void Update(float dt, const sf::Vector2i& mousePos, const std::vector<sf::Event>& events) override;
    virtual void Init() override;
};
