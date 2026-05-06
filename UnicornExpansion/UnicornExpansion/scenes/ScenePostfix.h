#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "SfmlGameEngine/Scene.h"
#include "SfmlGameEngine/Button.h"

class ScenePostfix: public sfge::Scene
{
private:
    sf::RectangleShape textback;
    sfge::UniqueText text_help;
    sfge::UniqueButton butok;
    std::string company ;
public:
    ScenePostfix(const std::string & company);
    virtual void Render(sf::RenderTarget& rendertarget) override;
    virtual void Update(float dt, const sf::Vector2i& mousePos, const std::vector<sf::Event>& events) override;
    virtual void Init() override;
};
