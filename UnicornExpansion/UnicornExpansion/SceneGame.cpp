#include "SceneGame.h"
#include "SfmlGameEngine/Engine.h"
#include "SceneMainMenu.h"

void SceneGame::Render(sf::RenderTarget & rendertarget) {
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
}

void SceneGame::UnInit() {
}