#include "SceneEndGame.h"
#include "SfmlGameEngine/Engine.h"
#include "SceneMainMenu.h"
#include "SceneGame.h"

SceneEndGame::SceneEndGame(const Game & game, int leveln) :Scene()
{
    iswin = game.isWin();
    this->leveln = leveln;
}

void SceneEndGame::Render(sf::RenderTarget & rendertarget) {
    rendertarget.draw(textback);
    rendertarget.draw(*text_title);
    for (auto & button: buttons) rendertarget.draw(*button);
}

void SceneEndGame::Update(float dt, const sf::Vector2i & mousePos, const std::vector<sf::Event>& events) {
    for (auto & event : events) {
        for (auto& button : buttons) button->processEvent(event);
        if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
            if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)	getEngine()->SwitchToScene(std::make_shared<SceneMainMenu>());
    }
}

void SceneEndGame::Init() {
    textback.setOutlineThickness(1);
    textback.setOutlineColor(sf::Color(192, 192, 192));
    textback.setFillColor(sf::Color{ 40, 40, 40, 192 });
    textback.setPosition({ 512 - 150, 200 });
    textback.setSize({ 300, 200 });

    text_title = loadText(getTexts().getStr(iswin ? "Msg_Win" : "Msg_Fail"), 22, sf::Color::White);
    text_title->setPosition({ 512 - text_title->getGlobalBounds().size.x/2, 220});
    
    if (iswin) {
        buttons.push_back(std::make_unique<sfge::Button>(*getEngine()->getDefaultFont(), getTexts().getSfmlStr("Text_Continue"), 18, 512 - 80, 280, 160, 40));
        buttons.back()->setOnClick([this]() { getEngine()->SwitchToScene(std::make_shared<SceneMainMenu>()); });
    }
    else {
        buttons.push_back(std::make_unique<sfge::Button>(*getEngine()->getDefaultFont(), getTexts().getSfmlStr("Text_Restart"), 18, 512 - 80, 280, 160, 40));
        buttons.back()->setOnClick([this]() { getEngine()->SwitchToScene(std::make_shared<SceneGame>(leveln)); });
    }
    buttons.push_back(std::make_unique<sfge::Button>(*getEngine()->getDefaultFont(), getTexts().getSfmlStr("Text_MainMenu"), 18, 512 - 80, 340, 160, 40));
    buttons.back()->setOnClick([this]() { getEngine()->SwitchToScene(std::make_shared<SceneMainMenu>()); });
}

void SceneEndGame::UnInit() {
}
