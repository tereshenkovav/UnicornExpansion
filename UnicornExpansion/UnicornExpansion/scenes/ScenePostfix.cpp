#include "ScenePostfix.h"
#include "SfmlGameEngine/Engine.h"
#include "SceneStartMenu.h"
#include "HelperCppClasses/StringTools.h"
#include <fstream>

ScenePostfix::ScenePostfix(const std::string & company) {
    this->company = company;
}

void ScenePostfix::Render(sf::RenderTarget & rendertarget) {
    rendertarget.draw(textback);
    rendertarget.draw(*text_help);

    rendertarget.draw(*butok);
}

void ScenePostfix::Update(float dt, const sf::Vector2i & mousePos, const std::vector<sf::Event>& events) {
    for (auto & event : events) {

        butok->processEvent(event);

        if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
        {
            if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)	
                getEngine()->ReplaceOverScene(std::make_shared<SceneStartMenu>());
        };
    }
}

void ScenePostfix::Init() {
    textback.setOutlineThickness(1);
    textback.setOutlineColor(sf::Color(192, 192, 192));
    textback.setFillColor(sf::Color{ 40, 40, 40, 192 });
    textback.setPosition({ 512 - 400, 200 });
    textback.setSize({ 800, 480 });
        
    auto str = readAllTextFromFile(std::format("company/{}/postfix.{}.txt", company, getEngine()->getLanguages().getCurrent()), "Unknown postfix", "\\n");
    text_help = loadText(replaceAllString(str, "\\n", "\n"), 20, sf::Color::White);
    text_help->setPosition({ 160, 220});

    butok = std::make_unique<sfge::Button>(*getEngine()->getDefaultFont(), "OK", 18,
        512 - 40, 700, 80, 40);
    butok->setOnClick([this]() {getEngine()->ReplaceOverScene(std::make_shared<SceneStartMenu>()); });
}
