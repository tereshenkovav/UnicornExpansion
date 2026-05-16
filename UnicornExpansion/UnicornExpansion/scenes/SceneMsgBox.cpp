#include "SceneMsgBox.h"
#include "SfmlGameEngine/Engine.h"
#include "HelperCppClasses/StringTools.h"

SceneMsgBox::SceneMsgBox(const std::string & str, std::function<void()> exitfunc) {
    this->str = str;
    this->exitfunc = exitfunc ;
}

void SceneMsgBox::Render(sf::RenderTarget & rendertarget) {
    rendertarget.draw(textback);
    rendertarget.draw(*text_help);

    rendertarget.draw(*butok);
}

void SceneMsgBox::Update(float dt, const sf::Vector2i & mousePos, const std::vector<sf::Event>& events) {
    for (auto & event : events) {

        butok->processEvent(event);

        if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
        {
            if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)	exitfunc();
        };
    }
}

void SceneMsgBox::Init() {
    textback.setOutlineThickness(1);
    textback.setOutlineColor(sf::Color(192, 192, 192));
    textback.setFillColor(sf::Color{ 40, 40, 40, 192 });
    textback.setPosition({ 512 - 400, 200 });
    textback.setSize({ 800, 480 });
        
    text_help = loadText(replaceAllString(str, "\\n", "\n"), 20, sf::Color::White);
    text_help->setPosition({ 160, 220});

    butok = std::make_unique<sfge::Button>(*getEngine()->getDefaultFont(), "OK", 18,
        512 - 40, 700, 80, 40);
    butok->setOnClick(exitfunc);
}
