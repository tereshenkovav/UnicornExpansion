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

    for (auto& button : buttons) rendertarget.draw(*button);
}

void SceneMainMenu::Update(float dt, const sf::Vector2i & mousePos, const std::vector<sf::Event>& events) {
    for (auto & event : events) {

        for (auto& button : buttons) button->processEvent(event);

        if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
        {
            if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)	getEngine()->doClose();
            if (keyPressed->scancode == sf::Keyboard::Scancode::M) switchSound();
        };
        if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>())
            snd_click->play();
    }
}

void SceneMainMenu::Init() {    
    spr_intro = loadSprite("images/intro.png");
    spr_title = loadSprite("images/title.png");
    spr_title->setOrigin({ (float)(spr_title->getTexture().getSize().x / 2), 0});
    spr_title->setPosition({ 512, 60 });
        
    text_help = loadText(getTexts().getStr("Text_Help"), 20, sf::Color::White);
    text_help->setPosition({ 670, 250 });
    
    text_version = loadText(VERSION, 28, getColors().getColor("textbackborder"));
    text_version->setPosition({ 1024 - 100, 768 - 50 });

    buttons.push_back(std::make_unique<sfge::Button>(*getEngine()->getDefaultFont(), getTexts().getSfmlStr("Text_Training"), 18,
        512 - 100, 300, 200, 40));
    buttons.back()->setOnClick([this]() {getEngine()->SwitchToScene(std::make_shared<SceneGame>(0)); });

    buttons.push_back(std::make_unique<sfge::Button>(*getEngine()->getDefaultFont(), getTexts().getSfmlStr("Text_Quit"), 18,
        512 - 100, 360, 200, 40));
    buttons.back()->setOnClick([this]() {getEngine()->doClose(); });

    snd_click = loadSound("sounds/click.ogg");
}

void SceneMainMenu::UnInit() {
}