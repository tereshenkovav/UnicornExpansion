#include "SceneGame.h"
#include "SfmlGameEngine/Engine.h"
#include "SceneMainMenu.h"
#include "SceneTask.h"

void SceneGame::Render(sf::RenderTarget & rendertarget) {
    rendertarget.draw(*spr_border);
}

void SceneGame::Update(float dt, const sf::Vector2i & mousePos, const std::vector<sf::Event>& events) {
    for (auto & event : events) {
        if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
        {
            if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) getEngine()->SwitchToScene(std::make_shared<SceneMainMenu>());
        };
    }
}

void SceneGame::Init() {
    spr_border = loadSprite("images/border.png");
    spr_border->setPosition({ 0, 768 - 192 });

    getEngine()->AddOverScene(std::make_shared<SceneTask>(getTexts().getStr("Text_Task") + "\n" + "Task123", sf::Color::Green,
        [this]()->void { getEngine()->doExitScene(); }));
}


void SceneGame::UnInit() {
}
