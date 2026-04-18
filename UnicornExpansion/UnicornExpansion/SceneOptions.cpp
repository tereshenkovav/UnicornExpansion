#include "SceneOptions.h"
#include "SfmlGameEngine/Engine.h"
#include "SceneStartMenu.h"

void SceneOptions::saveOptions() {
    getProfile()->setSoundOn(cbsound->isChecked());
    userprofile->setVoiceOn(cbvoice->isChecked());
    getProfile()->setFullScreen(cbfullscreen->isChecked());
    getProfile()->setVSync(cbvsync->isChecked());
    getEngine()->updateByProfile();
    getEngine()->ReplaceOverScene(std::make_shared<SceneStartMenu>());
}

void SceneOptions::Render(sf::RenderTarget & rendertarget) {
    rendertarget.draw(textback);
    rendertarget.draw(*text_caption);
    
    rendertarget.draw(*cbsound);
    rendertarget.draw(*cbvoice);
    rendertarget.draw(*cbfullscreen);
    rendertarget.draw(*cbvsync);

    rendertarget.draw(*butsave);
    rendertarget.draw(*butcancel);
}

void SceneOptions::Update(float dt, const sf::Vector2i & mousePos, const std::vector<sf::Event>& events) {
    for (auto & event : events) {

        butsave->processEvent(event);
        butcancel->processEvent(event);
        cbsound->processEvent(event);
        cbvoice->processEvent(event);
        cbfullscreen->processEvent(event);
        cbvsync->processEvent(event);

        cbvoice->setDisabled(!cbsound->isChecked());

        if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
        {
            if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)	
                getEngine()->ReplaceOverScene(std::make_shared<SceneStartMenu>());
        };
    }
}

void SceneOptions::Init() {
    userprofile = std::static_pointer_cast<UserProfile>(getProfile());

    textback.setOutlineThickness(1);
    textback.setOutlineColor(sf::Color(192, 192, 192));
    textback.setFillColor(sf::Color{ 40, 40, 40, 192 });
    textback.setPosition({ 512 - 220, 200 });
    textback.setSize({ 440, 480 });

    text_caption = loadText(getTexts().getStr("Text_Options"), 24, sf::Color::White);
    text_caption->setPosition({ 512 - text_caption->getGlobalBounds().size.x / 2, 220 });
        
    butsave = std::make_unique<sfge::Button>(*getEngine()->getDefaultFont(), getTexts().getSfmlStr("Text_Save"), 18,
        512 - 220, 700, 200, 40);
    butsave->setOnClick([this]() {saveOptions(); });

    butcancel = std::make_unique<sfge::Button>(*getEngine()->getDefaultFont(), getTexts().getSfmlStr("Text_Cancel"), 18,
        512 + 20, 700, 200, 40);
    butcancel->setOnClick([this]() {getEngine()->ReplaceOverScene(std::make_shared<SceneStartMenu>()); });

    cbsound = std::make_unique<sfge::Checkbox>(*getEngine()->getDefaultFont(), getTexts().getSfmlStr("Checkbox_Sound"), 18,
        512 - 160, 280, 24, 24);
    cbsound->setChecked(getProfile()->isSoundOn());
    cbvoice = std::make_unique<sfge::Checkbox>(*getEngine()->getDefaultFont(), getTexts().getSfmlStr("Checkbox_Voice"), 18,
        512 - 160, 320, 24, 24);
    cbvoice->setChecked(userprofile->isVoiceOn());
    cbfullscreen = std::make_unique<sfge::Checkbox>(*getEngine()->getDefaultFont(), getTexts().getSfmlStr("Checkbox_Fullscreen"), 18,
        512 - 160, 360, 24, 24);
    cbfullscreen->setChecked(getProfile()->isFullScreen());
    cbvsync = std::make_unique<sfge::Checkbox>(*getEngine()->getDefaultFont(), getTexts().getSfmlStr("Checkbox_VSync"), 18,
        512 - 160, 400, 24, 24);
    cbvsync->setChecked(getProfile()->isVSync());
}

void SceneOptions::UnInit() {
}