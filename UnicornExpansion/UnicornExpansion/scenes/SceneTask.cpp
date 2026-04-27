#include "SceneTask.h"
#include "SfmlGameEngine/Engine.h"

SceneTask::SceneTask(const Game & game) :Scene()
{
    tasks = game.getTasks();
}

void SceneTask::Render(sf::RenderTarget & rendertarget) {
    rendertarget.draw(textback);
    rendertarget.draw(*text_title);
    rendertarget.draw(*text_task);
    rendertarget.draw(*butok);
}

void SceneTask::Update(float dt, const sf::Vector2i & mousePos, const std::vector<sf::Event>& events) {
    for (auto & event : events) {
        butok->processEvent(event);
        if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
            if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)	getEngine()->doExitScene();
    }
}

void SceneTask::Init() {
    textback.setOutlineThickness(1);
    textback.setOutlineColor(sf::Color(192, 192, 192));
    textback.setFillColor(sf::Color{ 40, 40, 40, 192 });
    textback.setPosition({ 512 - 250, 100 });
    textback.setSize({ 500, 600 });

    text_title = loadText(getTexts().getStr("Text_Task"), 22, sf::Color::White);
    text_title->setPosition({ 512 - text_title->getGlobalBounds().size.x/2, 110});

    std::string task = "";
    for (auto& gtask : tasks) {
        if (gtask.status == GameTaskStatus::Completed) task += getTexts().getStr("Task_Completed") + ": ";
        if (gtask.status == GameTaskStatus::Cancelled) task += getTexts().getStr("Task_Cancelled") + ": ";
        task += gtask.text + "\n";
    }
    text_task = loadText(task, 18, sf::Color::White);
    text_task->setPosition({ 512 - 220, 150 });

    butok = std::make_unique<sfge::Button>(*getEngine()->getDefaultFont(), "OK", 22, 512 - 40, 640, 80, 40);
    butok->setOnClick([this]() { getEngine()->doExitScene(); });
}
