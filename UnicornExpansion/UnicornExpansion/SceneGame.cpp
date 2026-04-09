#include "SceneGame.h"
#include "SfmlGameEngine/Engine.h"
#include "SceneMainMenu.h"

void SceneGame::Render(sf::RenderTarget & rendertarget) {
    // Вывод курсора
    int delta = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) ? 4 : 0;
    cursor->setPosition({ (float)mousePos.x + delta,(float)mousePos.y + delta });
    rendertarget.draw(*cursor);
}

void SceneGame::Update(float dt, const sf::Vector2i & mousePos, const std::vector<sf::Event>& events) {
    this->mousePos = mousePos;
    for (auto & event : events) {
        if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
        {
            if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) engine->SwitchToScene(std::make_shared<SceneMainMenu>());
        };
    }
}

void SceneGame::Init() {
    textures.push_back(std::make_unique<sf::Texture>("images/cursor_my.png"));
    cursor = std::make_unique<sf::Sprite>(*textures.back());
}

void SceneGame::UnInit() {
}