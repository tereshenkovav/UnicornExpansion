#include "SceneMainMenu.h"
#include "version.h"
#include "SfmlGameEngine/Engine.h"
#include "SceneGame.h"
#include <SFML/Audio.hpp>

const int LEVEL_COUNT = 6;

void switchSound() {
    if (sf::Listener::getGlobalVolume() == 0.0f)
        sf::Listener::setGlobalVolume(100.0f);
    else
        sf::Listener::setGlobalVolume(0.0f);
}

void SceneMainMenu::Render(sf::RenderTarget & rendertarget) {
    rendertarget.draw(*spr_intro);
    rendertarget.draw(*spr_title);

    rendertarget.draw(*text_help);
    rendertarget.draw(*text_version);

        textback.setSize({ 240, 40 });
        for (int i = 0; i < LEVEL_COUNT; i++) {
            textback.setPosition({ 512 - 120, (float)(250 + 64 * i) });
            rendertarget.draw(textback);

            text_info->setString(getTexts().getSfmlStr("Name_Level_" + std::to_string(i)));
            text_info->setPosition({ 512 - text_info->getLocalBounds().size.x / 2, (float)(250 + 64 * i) + 8 });
            rendertarget.draw(*text_info);
        }
        textback.setPosition({ 512 - 120, (float)(250 + 64 * LEVEL_COUNT) });
        rendertarget.draw(textback);

        text_info->setString(getTexts().getSfmlStr("Text_Quit"));
        text_info->setPosition({ 512 - text_info->getLocalBounds().size.x / 2, (float)(250 + 64 * LEVEL_COUNT) + 8 });
        rendertarget.draw(*text_info);
}

void SceneMainMenu::Update(float dt, const sf::Vector2i & mousePos, const std::vector<sf::Event>& events) {
    std::optional<int> selected_idx;
    textback.setSize({ 240, 40 });
    for (int i = 0; i < LEVEL_COUNT; i++) {
        textback.setPosition({ 512 - 100, (float)(250 + 64 * i) });
        if (textback.getGlobalBounds().contains({ (float)mousePos.x, (float)mousePos.y })) selected_idx = i;
    }
    textback.setPosition({ 512 - 100, (float)(250 + 64 * LEVEL_COUNT) });
    if (textback.getGlobalBounds().contains({ (float)mousePos.x, (float)mousePos.y })) selected_idx = -1;

    if (selected_idx) getEngine()->setCursor(1);

    for (auto & event : events) {
        if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
        {
            if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)	getEngine()->doClose();
            if (keyPressed->scancode == sf::Keyboard::Scancode::M) switchSound();
        };
        if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>())
            snd_click->play();
        if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonReleased>())
        {
            if (selected_idx) {
                if (*selected_idx>=0)
                    getEngine()->SwitchToScene(std::make_shared<SceneGame>(*selected_idx));
                else
                if (*selected_idx < 0) getEngine()->doClose();
            }
        }
    }
}

void SceneMainMenu::Init() {
    textback.setOutlineThickness(1);
    textback.setOutlineColor(getColors().getColor("textbackborder"));
    textback.setFillColor(getColors().getColor("textbackfill"));

    spr_intro = loadSprite("images/intro.png");
    spr_title = loadSprite("images/title.png");
    spr_title->setOrigin({ (float)(spr_title->getTexture().getSize().x / 2), 0});
    spr_title->setPosition({ 512, 60 });
        
    text_help = loadText(getTexts().getStr("Text_Help"), 20, sf::Color::White);
    text_help->setPosition({ 670, 250 });
    
    text_version = loadText(VERSION, 28, getColors().getColor("textbackborder"));
    text_version->setPosition({ 1024 - 100, 768 - 50 });

    text_info = loadText(16, sf::Color::White);

    snd_click = loadSound("sounds/click.ogg");
}

void SceneMainMenu::UnInit() {
}