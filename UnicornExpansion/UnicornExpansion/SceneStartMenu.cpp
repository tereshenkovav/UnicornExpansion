#include "SceneStartMenu.h"
#include "version.h"
#include "SfmlGameEngine/Engine.h"
#include "SceneGame.h"
#include "SceneHelp.h"
#include <SFML/Audio.hpp>

void switchSound() {
    if (sf::Listener::getGlobalVolume() == 0.0f)
        sf::Listener::setGlobalVolume(100.0f);
    else
        sf::Listener::setGlobalVolume(0.0f);
}

void SceneStartMenu::Render(sf::RenderTarget & rendertarget) {
    rendertarget.draw(*text_caption);
    rendertarget.draw(*text_version);

    for (auto& button : buttons) rendertarget.draw(*button);
}

void SceneStartMenu::Update(float dt, const sf::Vector2i & mousePos, const std::vector<sf::Event>& events) {
    for (auto & event : events) {

        for (auto& button : buttons) button->processEvent(event);

        if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
        {
            if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)	getEngine()->doClose();
        };
        if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>())
            snd_click->play();
    }
}

void SceneStartMenu::Init() {
    text_caption = loadText(getTexts().getStr("Text_GameCaption1"), 24, sf::Color::White);
    text_caption->setPosition({ 512 - text_caption->getGlobalBounds().size.x / 2, 220 });
    
    text_version = loadText(VERSION, 28, getColors().getColor("textbackborder"));
    text_version->setPosition({ 512 - text_version->getGlobalBounds().size.x/2, 768 - 50});

    buttons.push_back(std::make_unique<sfge::Button>(*getEngine()->getDefaultFont(), getTexts().getSfmlStr("Text_Training"), 18,
        512 - 100, 320, 200, 40));
    buttons.back()->setOnClick([this]() {getEngine()->SwitchToScene(std::make_shared<SceneGame>(0)); });

    buttons.push_back(std::make_unique<sfge::Button>(*getEngine()->getDefaultFont(), getTexts().getSfmlStr("Text_DemoCompany"), 18,
        512 - 100, 380, 200, 40));
    buttons.back()->setOnClick([this]() {getEngine()->SwitchToScene(std::make_shared<SceneGame>(1)); });

    buttons.push_back(std::make_unique<sfge::Button>(*getEngine()->getDefaultFont(), getTexts().getSfmlStr("Text_Options"), 18,
        512 - 100, 440, 200, 40));
    buttons.back()->setOnClick([this]() {getEngine()->doClose(); });

    buttons.push_back(std::make_unique<sfge::Button>(*getEngine()->getDefaultFont(), getTexts().getSfmlStr("Text_Help"), 18,
        512 - 100, 500, 200, 40));
    buttons.back()->setOnClick([this]() {getEngine()->ReplaceOverScene(std::make_shared<SceneHelp>(HelpType::Help)); });

    buttons.push_back(std::make_unique<sfge::Button>(*getEngine()->getDefaultFont(), getTexts().getSfmlStr("Text_About"), 18,
        512 - 100, 560, 200, 40));
    buttons.back()->setOnClick([this]() {getEngine()->ReplaceOverScene(std::make_shared<SceneHelp>(HelpType::About)); });

    buttons.push_back(std::make_unique<sfge::Button>(*getEngine()->getDefaultFont(), getTexts().getSfmlStr("Text_Quit"), 18,
        512 - 100, 620, 200, 40));
    buttons.back()->setOnClick([this]() {getEngine()->doClose(); });

    snd_click = loadSound("sounds/click.ogg");
}

void SceneStartMenu::UnInit() {
}