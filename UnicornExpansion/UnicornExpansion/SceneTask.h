#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "SfmlGameEngine/Scene.h"

class SceneTask: public sfge::Scene
{
private:
    sf::RectangleShape textback;
    std::unique_ptr<sf::Text> text_task;
    std::unique_ptr<sf::Text> text_ok;
    std::string task;
    sf::Color color;
    std::function<void()> closefunc;
public:
    SceneTask(const std::string& task, sf::Color color, std::function<void()> closefunc);
    virtual void Render(sf::RenderTarget& rendertarget) override;
    virtual void Update(float dt, const sf::Vector2i& mousePos, const std::vector<sf::Event>& events) override;
    virtual void Init() override;
    virtual void UnInit() override;
};
