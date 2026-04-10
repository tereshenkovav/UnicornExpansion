#include "SceneMainMenu.h"
#include "version.h"
#include "SfmlGameEngine/Engine.h"
#include "SceneGame.h"

const int LEVEL_COUNT = 6;

void SceneMainMenu::Render(sf::RenderTarget & rendertarget) {
    rendertarget.draw(*spr_intro);
    rendertarget.draw(*spr_title);

    rendertarget.draw(*text_help);
    rendertarget.draw(*text_version);

        textback.setSize({ 240, 40 });
        for (int i = 0; i < LEVEL_COUNT; i++) {
            textback.setPosition({ 512 - 120, (float)(250 + 64 * i) });
            rendertarget.draw(textback);

            text_info->setString(texts.getSfmlStr("Name_Level_" + std::to_string(i)));
            text_info->setPosition({ 512 - text_info->getLocalBounds().size.x / 2, (float)(250 + 64 * i) + 8 });
            rendertarget.draw(*text_info);
        }
        textback.setPosition({ 512 - 120, (float)(250 + 64 * LEVEL_COUNT) });
        rendertarget.draw(textback);

        text_info->setString(texts.getSfmlStr("Text_Quit"));
        text_info->setPosition({ 512 - text_info->getLocalBounds().size.x / 2, (float)(250 + 64 * LEVEL_COUNT) + 8 });
        rendertarget.draw(*text_info);
    
    // Вывод курсора
    int delta = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) ? 4 : 0;
    cursor->setPosition({ (float)mousePos.x + delta,(float)mousePos.y + delta });
    rendertarget.draw(*cursor);
}

void SceneMainMenu::Update(float dt, const sf::Vector2i & mousePos, const std::vector<sf::Event>& events) {
    this->mousePos = mousePos;
    for (auto & event : events) {
        if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
        {
            if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)	getEngine()->doClose();
            //if (keyPressed->scancode == sf::Keyboard::Scancode::M) switchSound();
        };
        if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonReleased>())
        {
            textback.setSize({ 240, 40 });
            for (int i = 0; i < LEVEL_COUNT; i++) {
                textback.setPosition({ 512 - 100, (float)(250 + 64 * i) });
                if (textback.getGlobalBounds().contains({ (float)mousePos.x, (float)mousePos.y })) {
                    // Для выбора игры - переходим на сцену задания и загружаем игру                    
                    //loadGame(i);
                    getEngine()->SwitchToScene(std::make_shared<SceneGame>());
                }
            }
            textback.setPosition({ 512 - 100, (float)(250 + 64 * LEVEL_COUNT) });
            if (textback.getGlobalBounds().contains({ (float)mousePos.x, (float)mousePos.y })) getEngine()->doClose();
        }
    }
}

void SceneMainMenu::Init() {
    textback.setOutlineThickness(1);
    textback.setOutlineColor(sf::Color(192, 192, 192));
    textback.setFillColor(sf::Color{ 40, 40, 40, 192 });

    spr_intro = loadSprite("images/intro.png");
    spr_title = loadSprite("images/title.png");
    spr_title->setOrigin({ (float)(spr_title->getTexture().getSize().x / 2), 0});
    spr_title->setPosition({ 512, 60 });
        
    texts.loadFromFile("strings.txt");
        
    text_help = loadText(texts.getStr("Text_Help"), 20, sf::Color::White);
    text_help->setPosition({ 670, 250 });
    
    text_version = loadText(VERSION, 28, sf::Color({ 192,192,192 }));
    text_version->setPosition({ 1024 - 100, 768 - 50 });

    text_info = loadText(16, sf::Color::White);

    cursor = loadSprite("images/cursor_def.png");
}

void SceneMainMenu::UnInit() {
}