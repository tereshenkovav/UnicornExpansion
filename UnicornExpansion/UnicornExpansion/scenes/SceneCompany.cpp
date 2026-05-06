#include "SceneCompany.h"
#include "SfmlGameEngine/Engine.h"
#include "SceneMainMenu.h"
#include "HelperCppClasses/StringTools.h"
#include "SfmlGameEngine/Texts.h"
#include "SfmlGameEngine/SfmlTools.h"
#include "SceneGame.h"

void SceneCompany::updateCheckboxes()
{
    cbeasy->setChecked(difficulty == Difficulty::Easy);
    cbnorm->setChecked(difficulty == Difficulty::Norm);
    cbhard->setChecked(difficulty == Difficulty::Hard);
}

SceneCompany::SceneCompany(const std::string& company)
{
    this->company = company;
}

void SceneCompany::Render(sf::RenderTarget & rendertarget) {
    rendertarget.draw(*spr_intro);

    rendertarget.draw(textback);
    rendertarget.draw(textback1);
    rendertarget.draw(*text_caption);
    rendertarget.draw(*text_info);
    
    rendertarget.draw(*cbeasy);
    rendertarget.draw(*cbnorm);
    rendertarget.draw(*cbhard);
    rendertarget.draw(*butcancel);
    for (auto& button : buttons)
        rendertarget.draw(*button);
}

void SceneCompany::Update(float dt, const sf::Vector2i & mousePos, const std::vector<sf::Event>& events) {
    for (auto & event : events) {

        butcancel->processEvent(event);
        
        bool v = cbeasy->isChecked();
        cbeasy->processEvent(event);
        if ((!v) && cbeasy->isChecked()) {
            cbnorm->setChecked(false);
            cbhard->setChecked(false);
        }
        else {
            if (v && (!cbeasy->isChecked())) cbeasy->setChecked(true);
        }

        v = cbnorm->isChecked();
        cbnorm->processEvent(event);
        if ((!v) && cbnorm->isChecked()) {
            cbeasy->setChecked(false);
            cbhard->setChecked(false);
        }
        else {
            if (v && (!cbnorm->isChecked())) cbnorm->setChecked(true);
        }
        
        v = cbhard->isChecked();
        cbhard->processEvent(event);
        if ((!v) && cbhard->isChecked()) {
            cbnorm->setChecked(false);
            cbeasy->setChecked(false);
        }
        else {
            if (v && (!cbhard->isChecked())) cbhard->setChecked(true);
        }
        
        for (auto& button : buttons)
            button->processEvent(event);
                
        if (cbeasy->isChecked()) difficulty = Difficulty::Easy;
        if (cbnorm->isChecked()) difficulty = Difficulty::Norm;
        if (cbhard->isChecked()) difficulty = Difficulty::Hard;
        
        if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
        {
            if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                getEngine()->SwitchToScene(std::make_shared<SceneMainMenu>());
        };
    }
}

void SceneCompany::Init() {
    userprofile = std::static_pointer_cast<UserProfile>(getProfile());

    spr_intro = loadSprite("images/intro.png");

    textback.setOutlineThickness(1);
    textback.setOutlineColor(sf::Color(192, 192, 192));
    textback.setFillColor(sf::Color{ 40, 40, 40, 192 });
    textback.setPosition({ 16 , 16 });
    textback.setSize({ 600, 768 - 32 });

    textback1.setOutlineThickness(1);
    textback1.setOutlineColor(sf::Color(192, 192, 192));
    textback1.setFillColor(sf::Color{ 40, 40, 40, 192 });
    textback1.setPosition({ 600 + 16 + 16 , 16 });
    textback1.setSize({ 376, 768 - 32 });

    text_caption = loadText(readAllTextFromFile(std::format("company/{}/name.txt", company), "Unknown company"), 24, sf::Color::White);
    text_caption->setPosition({ 16 + 300 - text_caption->getGlobalBounds().size.x / 2, 32 });
    
    auto str = readAllTextFromFile(std::format("company/{}/descr.txt", company), "Unknown description");
    text_info = loadText(replaceAllString(str, "\\n", "\n"), 20, sf::Color(192, 192, 192));
    text_info->setPosition({ 32, 100 });

    butcancel = std::make_unique<sfge::Button>(*getEngine()->getDefaultFont(), getTexts().getSfmlStr("Text_MainMenu"), 18,
        1024-116-16, 700, 100, 40);
    butcancel->setOnClick([this]() {getEngine()->SwitchToScene(std::make_shared<SceneMainMenu>()); });

    difficulty = Difficulty::Norm;

    cbeasy = std::make_unique<sfge::Checkbox>(*getEngine()->getDefaultFont(), getTexts().getSfmlStr("Checkbox_Easy"), 18,
        80, 700, 24, 24);
    cbnorm = std::make_unique<sfge::Checkbox>(*getEngine()->getDefaultFont(), getTexts().getSfmlStr("Checkbox_Norm"), 18,
        240, 700, 24, 24);
    cbhard = std::make_unique<sfge::Checkbox>(*getEngine()->getDefaultFont(), getTexts().getSfmlStr("Checkbox_Hard"), 18,
        400, 700, 24, 24);
    updateCheckboxes();

    companylevels.clear();
    sfge::Texts text;
    for (int i = 0; i<UserProfile::getLevelCount(company); i++) {
        text.loadFromFile(std::format("company/{}/level{}.strings", company, i));
        companylevels.push_back(std::format("{}. {}", i + 1, text.getStr("MapName")));
    }

    for (int i = 0; i < companylevels.size(); i++)
        if (i<=userprofile->getNextLevel(company)) {
            buttons.push_back(std::make_unique<sfge::Button>(*getEngine()->getDefaultFont(), sfge::SfmlTools::utf2text(companylevels[i]), 18,
                600 + 16 + 16 + 38, 64 + i * 54, 300, 40));
            buttons.back()->setOnClick([this, i]() {getEngine()->SwitchToScene(std::make_shared<SceneGame>(company, i)); });
        }
}
