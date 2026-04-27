#include "SceneGameMenu.h"
#include "SfmlGameEngine/Engine.h"
#include "SceneMainMenu.h"
#include "SceneGame.h"
#include "SceneTask.h"
#include "SceneJournal.h"

SceneGameMenu::SceneGameMenu(Game * game, LevelCode levelcode) :Scene()
{
    this->game = game;
    this->levelcode = levelcode;
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
    textback.setSize({ 240, 320 });
    
    buttons.push_back(std::make_unique<sfge::Button>(*getEngine()->getDefaultFont(), getTexts().getSfmlStr("Text_Continue"), 18, 512 - 90, 200, 180, 40));
    buttons.back()->setOnClick([this]() { getEngine()->doExitScene(); });
    buttons.push_back(std::make_unique<sfge::Button>(*getEngine()->getDefaultFont(), getTexts().getSfmlStr("Text_Restart"), 18, 512 - 90, 260, 180, 40));
    buttons.back()->setOnClick([this]() { getEngine()->SwitchToScene(std::make_shared<SceneGame>(levelcode)); });
    buttons.push_back(std::make_unique<sfge::Button>(*getEngine()->getDefaultFont(), getTexts().getSfmlStr("Text_Task"), 18, 512 - 90, 320, 180, 40));
    buttons.back()->setOnClick([this]() { getEngine()->ReplaceOverScene(std::make_shared<SceneTask>(*game)); });
    buttons.push_back(std::make_unique<sfge::Button>(*getEngine()->getDefaultFont(), getTexts().getSfmlStr("Text_Journal"), 18, 512 - 90, 380, 180, 40));
    buttons.back()->setOnClick([this]() { getEngine()->ReplaceOverScene(std::make_shared<SceneJournal>(*game)); });
    buttons.push_back(std::make_unique<sfge::Button>(*getEngine()->getDefaultFont(), getTexts().getSfmlStr("Text_MainMenu"), 18, 512 - 90, 440, 180, 40));
    buttons.back()->setOnClick([this]() { getEngine()->SwitchToScene(std::make_shared<SceneMainMenu>()); });
}
