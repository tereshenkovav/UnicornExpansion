#include "SceneHelp.h"
#include "SfmlGameEngine/Engine.h"
#include "SceneStartMenu.h"
#include <fstream>

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
    
    std::string str = "";
    if (helptype == HelpType::Help) {
        str = getTexts().getStr("Text_FullHelp");
    }
    else {
        str = getTexts().getStr("Text_FullAbout");
        if (std::filesystem::exists("credits.txt")) {
            std::string line;
            std::ifstream fin("credits.txt");
            if (fin.is_open()) {
                str += "\n\n" + getTexts().getStr("Text_Credits") + "\n\n";
                std::vector<std::string> lines;
                while (std::getline(fin, line))
                    lines.push_back(line);
                for (int i = 0; i < lines.size(); i++) {
                    str += lines[i];
                    if (i < lines.size() - 1) str += ((i + 1) % 4 == 0) ? "\n" : ", ";
                }
                fin.close();
            }
        }
    }
    text_help = loadText(str, 20, sf::Color::White);
    text_help->setPosition({ 160, 280});

    butok = std::make_unique<sfge::Button>(*getEngine()->getDefaultFont(), "OK", 18,
        512 - 40, 700, 80, 40);
    butok->setOnClick([this]() {getEngine()->ReplaceOverScene(std::make_shared<SceneStartMenu>()); });
}

void SceneHelp::UnInit() {
}