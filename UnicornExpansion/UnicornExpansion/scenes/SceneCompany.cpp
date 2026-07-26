#include "SceneCompany.h"
#include "SfmlGameEngine/Engine.h"
#include "SceneMainMenu.h"
#include "HelperCppClasses/StringTools.h"
#include "SfmlGameEngine/Texts.h"
#include "SfmlGameEngine/SfmlTools.h"
#include "SceneGame.h"

SceneCompany::SceneCompany(const std::string& company)
{
    this->company = company;
}

void SceneCompany::Render(sf::RenderTarget & rendertarget) {
    rendertarget.draw(*spr_intro);

    rendertarget.draw(textback);
    rendertarget.draw(textback1);
    rendertarget.draw(*text_caption);

    drawTextInBlockWidth(rendertarget, *text_info, companyinfo, 32, 100, 600-32, 5);
    
    rendertarget.draw(*cbgroup);
    rendertarget.draw(*butcancel);
    for (auto& button : buttons)
        rendertarget.draw(*button);
}

void SceneCompany::Update(float dt, const sf::Vector2i & mousePos, const std::vector<sf::Event>& events) {
    for (auto & event : events) {
                
        butcancel->processEvent(event);
        cbgroup->processEvent(event);

        for (auto& button : buttons)
            button->processEvent(event);
        
        if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
        {
            if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                getEngine()->SwitchToScene(std::make_shared<SceneMainMenu>());
        };
    }
        
    if ((Difficulty)cbgroup->getCheckedIdx() != userprofile->getLastDifficulty()) {
        userprofile->setLastDifficulty((Difficulty)cbgroup->getCheckedIdx());
        userprofile->saveProfile();
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

    text_caption = loadText(readAllTextFromFile(std::format("company/{}/name.{}.txt", company,getEngine()->getLanguages().getCurrent()), "Unknown company"), 24, sf::Color::White);
    text_caption->setPosition({ 16 + 300 - text_caption->getGlobalBounds().size.x / 2, 32 });
    
    companyinfo = readAllTextFromFile(std::format("company/{}/descr.{}.txt", company, getEngine()->getLanguages().getCurrent()), "Unknown description", "\\n");
    text_info = loadText(20, sf::Color(192, 192, 192));
    
    butcancel = std::make_unique<sfge::Button>(*getEngine()->getDefaultFont(), getTexts().getSfmlStr("Text_MainMenu"), 18,
        1024-116-16, 700, 100, 40);
    butcancel->setOnClick([this]() {getEngine()->SwitchToScene(std::make_shared<SceneMainMenu>()); });

    cbgroup = std::make_unique<sfge::CheckboxGroup>();
    cbgroup->addCheckbox(*getEngine()->getDefaultFont(), getTexts().getSfmlStr("Checkbox_Easy"), 18, 80, 700, 24, 24);
    cbgroup->addCheckbox(*getEngine()->getDefaultFont(), getTexts().getSfmlStr("Checkbox_Norm"), 18, 240, 700, 24, 24);
    cbgroup->addCheckbox(*getEngine()->getDefaultFont(), getTexts().getSfmlStr("Checkbox_Hard"), 18, 400, 700, 24, 24);
    cbgroup->setCheckedIdx((int)userprofile->getLastDifficulty());
    
    companylevels.clear();
    sfge::Texts text;
    for (int i = 0; i<UserProfile::getLevelCount(company); i++) {
        text.loadFromFile(std::format("company/{}/level{}.{}.strings", company, i, getEngine()->getLanguages().getCurrent()));
        companylevels.push_back(std::format("{}. {}", i + 1, text.getStr("MapName")));
    }

    for (int i = 0; i < companylevels.size(); i++)
        if (i<=userprofile->getNextLevel(company)) {
            buttons.push_back(std::make_unique<sfge::Button>(*getEngine()->getDefaultFont(), sfge::SfmlTools::utf2text(companylevels[i]), 18,
                600 + 16 + 16 + 38, 64 + i * 54, 300, 40));
            buttons.back()->setOnClick([this, i]() {getEngine()->SwitchToScene(std::make_shared<SceneGame>(company, i, userprofile->getLastDifficulty())); });
        }
}
