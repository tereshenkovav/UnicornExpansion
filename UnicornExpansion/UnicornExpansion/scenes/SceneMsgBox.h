#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "SfmlGameEngine/Scene.h"
#include "SfmlGameEngine/Button.h"

class SceneMsgBox: public sfge::Scene
{
private:
    sf::RectangleShape textback;
    sfge::UniqueText text_help;
    sfge::UniqueButton butok;
    std::string str;
    std::function<void()> exitfunc;
public:
    SceneMsgBox(const std::string& str, std::function<void()> exitfunc);
    virtual void Render(sf::RenderTarget& rendertarget) override;
    virtual void Update(float dt, const sf::Vector2i& mousePos, const std::vector<sf::Event>& events) override;
    virtual void Init() override;
};
