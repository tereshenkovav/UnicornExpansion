#include "SceneHelp.h"
#include "SfmlGameEngine/Engine.h"
#include "SceneStartMenu.h"

SceneHelp::SceneHelp(HelpType helptype) {
    this->helptype = helptype;
}

void SceneHelp::Render(sf::RenderTarget & rendertarget) {
    rendertarget.draw(textback);
    rendertarget.draw(*text_caption);
    rendertarget.draw(*text_help);

    rendertarget.draw(*butok);
}

void SceneHelp::Update(float dt, const sf::Vector2i & mousePos, const std::vector<sf::Event>& events) {
    for (auto & event : events) {

        butok->processEvent(event);

        if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
        {
            if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)	
                getEngine()->ReplaceOverScene(std::make_shared<SceneStartMenu>());
        };
    }
}

void SceneHelp::Init() {
    textback.setOutlineThickness(1);
    textback.setOutlineColor(sf::Color(192, 192, 192));
    textback.setFillColor(sf::Color{ 40, 40, 40, 192 });
    textback.setPosition({ 512 - 400, 200 });
    textback.setSize({ 800, 480 });

    text_caption = loadText(getTexts().getStr(helptype == HelpType::Help?"Text_HelpCaption":"Text_About"), 24, sf::Color::White);
    text_caption->setPosition({ 512 - text_caption->getGlobalBounds().size.x / 2, 220 });
    
    text_help = loadText(getTexts().getStr(helptype == HelpType::Help?"Text_FullHelp":"Text_FullAbout"), 20, sf::Color::White);
    text_help->setPosition({ 160, 280});

    butok = std::make_unique<sfge::Button>(*getEngine()->getDefaultFont(), "OK", 18,
        512 - 40, 700, 80, 40);
    butok->setOnClick([this]() {getEngine()->ReplaceOverScene(std::make_shared<SceneStartMenu>()); });
}

void SceneHelp::UnInit() {
}