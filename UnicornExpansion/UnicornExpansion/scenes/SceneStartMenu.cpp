#include "SceneStartMenu.h"
#include "version.h"
#include "SfmlGameEngine/Engine.h"
#include "SceneGame.h"
#include "SceneHelp.h"
#include "SceneOptions.h"
#include "SceneCompany.h"
#include "SceneMainMenu.h"

void SceneStartMenu::SwitchLanguage() {
    getEngine()->getLanguages().switchCurrent();
    getEngine()->getProfile()->setLanguage(getEngine()->getLanguages().getCurrent());
    std::static_pointer_cast<UserProfile>(getProfile())->saveProfile();
    getEngine()->loadTexts("strings.txt");
    getEngine()->setCaption(getEngine()->getTexts().getStr("Text_GameCaption"));
    getEngine()->SwitchToScene(std::make_shared<SceneMainMenu>());
}

void SceneStartMenu::Render(sf::RenderTarget & rendertarget) {
    rendertarget.draw(*text_caption);
    rendertarget.draw(*text_version);

    for (auto& button : buttons) rendertarget.draw(*button);

    rendertarget.draw(*spr_lang);
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
    text_caption->setPosition({ 512 - text_caption->getGlobalBounds().size.x / 2, 200 });
    
    text_version = loadText(VERSION, 28, getColors().getColor("textbackborder"));
    text_version->setPosition({ 512 - text_version->getGlobalBounds().size.x/2, 768 - 50});

    spr_lang = loadSprite("images/lang.png");

    buttons.clear();

    buttons.push_back(std::make_unique<sfge::Button>(*getEngine()->getDefaultFont(), getTexts().getSfmlStr("Text_Training"), 18,
        512 - 100, 260, 200, 40));
    buttons.back()->setOnClick([this]() {getEngine()->SwitchToScene(std::make_shared<SceneGame>("tutorial", 0, Difficulty::Norm)); });

    buttons.push_back(std::make_unique<sfge::Button>(*getEngine()->getDefaultFont(), getTexts().getSfmlStr("Text_DemoCompany"), 18,
        512 - 100, 315, 200, 40));
    buttons.back()->setOnClick([this]() {getEngine()->SwitchToScene(std::make_shared<SceneCompany>("demo")); });

    buttons.push_back(std::make_unique<sfge::Button>(*getEngine()->getDefaultFont(), getTexts().getSfmlStr("Text_Challenge"), 18,
        512 - 100, 370, 200, 40));
    buttons.back()->setOnClick([this]() {getEngine()->SwitchToScene(std::make_shared<SceneCompany>("challenge")); });

    buttons.push_back(std::make_unique<sfge::Button>(*getEngine()->getDefaultFont(), getTexts().getSfmlStr("Text_Options"), 18,
        512 - 100, 425, 200, 40));
    buttons.back()->setOnClick([this]() {getEngine()->ReplaceOverScene(std::make_shared<SceneOptions>()); });

    buttons.push_back(std::make_unique<sfge::Button>(*getEngine()->getDefaultFont(),
        getTexts().getSfmlStr("Text_Language")+": "+getEngine()->getLanguages().getCurrentUpper(), 18,
        512 - 100, 480, 200, 40));
    buttons.back()->setOnClick([this]() {SwitchLanguage(); });
    spr_lang->setPosition({ 512 + 64, 480 + (40 - spr_lang->getTexture().getSize().y)/2.0f});

    buttons.push_back(std::make_unique<sfge::Button>(*getEngine()->getDefaultFont(), getTexts().getSfmlStr("Text_Help"), 18,
        512 - 100, 535, 200, 40));
    buttons.back()->setOnClick([this]() {getEngine()->ReplaceOverScene(std::make_shared<SceneHelp>(HelpType::Help)); });

    buttons.push_back(std::make_unique<sfge::Button>(*getEngine()->getDefaultFont(), getTexts().getSfmlStr("Text_About"), 18,
        512 - 100, 590, 200, 40));
    buttons.back()->setOnClick([this]() {getEngine()->ReplaceOverScene(std::make_shared<SceneHelp>(HelpType::About)); });

    buttons.push_back(std::make_unique<sfge::Button>(*getEngine()->getDefaultFont(), getTexts().getSfmlStr("Text_Quit"), 18,
        512 - 100, 645, 200, 40));
    buttons.back()->setOnClick([this]() {getEngine()->doClose(); });

    snd_click = loadSound("sounds/click.ogg");
}
