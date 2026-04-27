#include "SceneJournal.h"
#include "SfmlGameEngine/Engine.h"

SceneJournal::SceneJournal(const Game & game) :Scene()
{
    msg = "";
    int cnt = 0;
    for (int i = game.getHistory().size()-1; i >= 0; i--) {
        msg = std::format("\t{}. {}\n", i + 1, game.getHistory()[i].text) + msg;
        if (++cnt == 6) {
            if (i > 0) msg = "\t...\n" + msg;
            break;
        }
    }
}

void SceneJournal::Render(sf::RenderTarget & rendertarget) {
    rendertarget.draw(textback);
    rendertarget.draw(*text_title);
    rendertarget.draw(*text_msgs);
    rendertarget.draw(*butok);
}

void SceneJournal::Update(float dt, const sf::Vector2i & mousePos, const std::vector<sf::Event>& events) {
    for (auto & event : events) {
        butok->processEvent(event);
        if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
            if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)	getEngine()->doExitScene();
    }
}

void SceneJournal::Init() {
    textback.setOutlineThickness(1);
    textback.setOutlineColor(sf::Color(192, 192, 192));
    textback.setFillColor(sf::Color{ 40, 40, 40, 192 });
    textback.setPosition({ 512 - 350, 100 });
    textback.setSize({ 700, 600 });

    text_title = loadText(getTexts().getStr("Text_Journal"), 22, sf::Color::White);
    text_title->setPosition({ 512 - text_title->getGlobalBounds().size.x/2, 110});
    
    text_msgs = loadText(msg, 18, sf::Color::White);
    text_msgs->setPosition({ 1024 / 2 - 350 + 50, 150 });

    butok = std::make_unique<sfge::Button>(*getEngine()->getDefaultFont(), "OK", 22, 512 - 40, 640, 80, 40);
    butok->setOnClick([this]() { getEngine()->doExitScene(); });
}
