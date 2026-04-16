#include "SceneGameMenu.h"
#include "SfmlGameEngine/Engine.h"
#include "SceneMainMenu.h"
#include "SceneGame.h"
#include "SceneTask.h"

SceneGameMenu::SceneGameMenu(Game * game, int leveln) :Scene()
{
    this->game = game;
    this->leveln = leveln;
}

void SceneGameMenu::Render(sf::RenderTarget & rendertarget) {
    rendertarget.draw(textback);
    for (auto & button: buttons) rendertarget.draw(*button);
}

void SceneGameMenu::Update(float dt, const sf::Vector2i & mousePos, const std::vector<sf::Event>& events) {
    for (auto & event : events) {
        for (auto& button : buttons) button->processEvent(event);
        if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
            if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)	getEngine()->doExitScene();
    }
}

void SceneGameMenu::Init() {
    textback.setOutlineThickness(1);
    textback.setOutlineColor(sf::Color(192, 192, 192));
    textback.setFillColor(sf::Color{ 40, 40, 40, 192 });
    textback.setPosition({ 512 - 120, 180 });
    textback.setSize({ 240, 260 });
    
    buttons.push_back(std::make_unique<sfge::Button>(*getEngine()->getDefaultFont(), getTexts().getSfmlStr("Text_Continue"), 18, 512 - 80, 200, 160, 40));
    buttons.back()->setOnClick([this]() { getEngine()->doExitScene(); });
    buttons.push_back(std::make_unique<sfge::Button>(*getEngine()->getDefaultFont(), getTexts().getSfmlStr("Text_Restart"), 18, 512 - 80, 260, 160, 40));
    buttons.back()->setOnClick([this]() { getEngine()->SwitchToScene(std::make_shared<SceneGame>(leveln)); });
    buttons.push_back(std::make_unique<sfge::Button>(*getEngine()->getDefaultFont(), getTexts().getSfmlStr("Text_Task"), 18, 512 - 80, 320, 160, 40));
    buttons.back()->setOnClick([this]() { getEngine()->ReplaceOverScene(std::make_shared<SceneTask>(*game)); });
    buttons.push_back(std::make_unique<sfge::Button>(*getEngine()->getDefaultFont(), getTexts().getSfmlStr("Text_MainMenu"), 18, 512 - 80, 380, 160, 40));
    buttons.back()->setOnClick([this]() { getEngine()->SwitchToScene(std::make_shared<SceneMainMenu>()); });
}

void SceneGameMenu::UnInit() {
}
