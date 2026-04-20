#include "SceneCloseHandler.h"
#include "SfmlGameEngine/Engine.h"

void SceneCloseHandler::Render(sf::RenderTarget & rendertarget) {
    textback.setPosition({ 0, 0 });
    textback.setSize({ 1024, 768 });
    rendertarget.draw(textback);

    textback.setPosition({ 512 - 220, 200 });
    textback.setSize({ 440, 140 });
    rendertarget.draw(textback);

    rendertarget.draw(*text_caption);
    
    rendertarget.draw(*butconfirm);
    rendertarget.draw(*butcancel);
}

void SceneCloseHandler::Update(float dt, const sf::Vector2i & mousePos, const std::vector<sf::Event>& events) {
    for (auto & event : events) {

        butconfirm->processEvent(event);
        butcancel->processEvent(event);

        if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
            if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)	
                getEngine()->doExitScene() ;
    }
}

void SceneCloseHandler::Init() {
    textback.setOutlineThickness(1);
    textback.setOutlineColor(sf::Color(192, 192, 192));
    textback.setFillColor(sf::Color{ 40, 40, 40, 192 });
    
    text_caption = loadText(getTexts().getStr("Text_CloseMsg"), 24, sf::Color::White);
    text_caption->setPosition({ 512 - text_caption->getGlobalBounds().size.x / 2, 220 });
        
    butconfirm = std::make_unique<sfge::Button>(*getEngine()->getDefaultFont(), getTexts().getSfmlStr("Text_ConfirmClose"), 18,
        512 - 180, 280, 160, 40);
    butconfirm->setOnClick([this]() {getEngine()->doClose(); });

    butcancel = std::make_unique<sfge::Button>(*getEngine()->getDefaultFont(), getTexts().getSfmlStr("Text_Cancel"), 18,
        512 + 20, 280, 160, 40);
    butcancel->setOnClick([this]() {getEngine()->doExitScene(); });
}

void SceneCloseHandler::UnInit() {
}