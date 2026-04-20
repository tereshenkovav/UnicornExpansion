#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "SfmlGameEngine/Scene.h"
#include "SfmlGameEngine/Button.h"

enum HelpType { Help, About };

class SceneHelp: public sfge::Scene
{
private:
    sf::RectangleShape textback;
    std::unique_ptr<sf::Text> text_caption;
    std::unique_ptr<sf::Text> text_help;
    std::unique_ptr<sfge::Button> butok;
    HelpType helptype;
public:
    SceneHelp(HelpType helptype);
    virtual void Render(sf::RenderTarget& rendertarget) override;
    virtual void Update(float dt, const sf::Vector2i& mousePos, const std::vector<sf::Event>& events) override;
    virtual void Init() override;
};
