#include "SceneTask.h"
#include "SfmlGameEngine/Engine.h"

SceneTask::SceneTask(const std::string& task):Scene()
{
    this->task = task;
}

void SceneTask::Render(sf::RenderTarget & rendertarget) {
    textback.setPosition({ 512 - 200, 286 });
    textback.setSize({ 400, 150 });
    rendertarget.draw(textback);

    rendertarget.draw(*text_task);

    textback.setPosition({ 512 - 40, 400 });
    textback.setSize({ 80, 30 });
    rendertarget.draw(textback);

    rendertarget.draw(*text_ok);
}

void SceneTask::Update(float dt, const sf::Vector2i & mousePos, const std::vector<sf::Event>& events) {
    std::optional<int> sel = std::nullopt;
    
    textback.setPosition({ 512 - 40, 400 });
    textback.setSize({ 80, 30 });
    if (textback.getGlobalBounds().contains({ (float)mousePos.x, (float)mousePos.y })) sel=1;

    if (sel) getEngine()->setCursor(1);

    for (auto & event : events) {
        if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
        {
            if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)	getEngine()->doExitScene();
        };
        if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonReleased>())
        {
            if (sel) getEngine()->doExitScene();
        }
    }
}

void SceneTask::Init() {
    textback.setOutlineThickness(1);
    textback.setOutlineColor(sf::Color(192, 192, 192));
    textback.setFillColor(sf::Color{ 40, 40, 40, 192 });
    
    text_task = loadText(getTexts().getStr("Text_Task") + "\n" + task, 22);    
    text_task->setPosition({ 512 - 190, 290 });

    text_ok = loadText("OK", 24, sf::Color::White);    
    text_ok->setPosition({ 512 - 20, 400 });
}

void SceneTask::UnInit() {
}
