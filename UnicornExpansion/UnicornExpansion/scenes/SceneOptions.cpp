#include "SceneOptions.h"
#include "SfmlGameEngine/Engine.h"
#include "SceneStartMenu.h"

void SceneOptions::saveOptions() {
    getProfile()->setSoundOn(cbsound->isChecked());
    userprofile->setVoiceOn(cbvoice->isChecked());
    userprofile->setHideFullHealthBar(cbhidefullhealthbar->isChecked());
    getProfile()->setFullScreen(cbfullscreen->isChecked());
    getProfile()->setVSync(cbvsync->isChecked());
    getProfile()->setFPS(fps);
    getProfile()->setStopGameOnLostFocus(cbstopgameonlostfocus->isChecked());
    getEngine()->updateByProfile();
    getEngine()->ReplaceOverScene(std::make_shared<SceneStartMenu>());
    userprofile->saveProfile();
}

void SceneOptions::updateFPSText()
{
    text_fps->setString("FPS: " + std::to_string(fps));
}

void SceneOptions::Render(sf::RenderTarget & rendertarget) {
    rendertarget.draw(textback);
    rendertarget.draw(*text_caption);
    
    rendertarget.draw(*cbsound);
    rendertarget.draw(*cbvoice);
    rendertarget.draw(*cbfullscreen);
    rendertarget.draw(*cbvsync);

    rendertarget.draw(*butincfps);
    rendertarget.draw(*butdecfps);
    rendertarget.draw(*text_fps);

    rendertarget.draw(*cbhidefullhealthbar);
    rendertarget.draw(*cbstopgameonlostfocus);

    rendertarget.draw(*butsave);
    rendertarget.draw(*butcancel);
}

void SceneOptions::Update(float dt, const sf::Vector2i & mousePos, const std::vector<sf::Event>& events) {
    cbvoice->setDisabled(!cbsound->isChecked());
    butdecfps->setDisabled(cbvsync->isChecked());
    butincfps->setDisabled(cbvsync->isChecked());
    text_fps->setFillColor(cbvsync->isChecked() ? sf::Color({ 128, 128, 128 }) : sf::Color::White);

    for (auto & event : events) {

        butsave->processEvent(event);
        butcancel->processEvent(event);
        cbsound->processEvent(event);
        cbvoice->processEvent(event);
        cbfullscreen->processEvent(event);
        cbvsync->processEvent(event);
        cbhidefullhealthbar->processEvent(event);
        cbstopgameonlostfocus->processEvent(event);
        butincfps->processEvent(event);
        butdecfps->processEvent(event);

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
    cbhidefullhealthbar = std::make_unique<sfge::Checkbox>(*getEngine()->getDefaultFont(), getTexts().getSfmlStr("Checkbox_HideFullHealthBar"), 18,
        512 - 160, 480, 24, 24);
    cbhidefullhealthbar->setChecked(userprofile->isHideFullHealthBar());
    cbstopgameonlostfocus = std::make_unique<sfge::Checkbox>(*getEngine()->getDefaultFont(), getTexts().getSfmlStr("Checkbox_StopGameOnLostFocus"), 18,
        512 - 160, 520, 24, 24);
    cbstopgameonlostfocus->setChecked(userprofile->isStopGameOnLostFocus());

    fps = getProfile()->getFPS();
    text_fps = loadText("", 18, sf::Color::White);
    text_fps->setPosition({ 512 - 80, 440 });

    updateFPSText();

    butdecfps = std::make_unique<sfge::Button>(*getEngine()->getDefaultFont(), "-", 18, 512 - 120, 440, 24, 24);
    butdecfps->setOnClick([this]() { if (fps > 10) fps--; updateFPSText(); });
    butincfps = std::make_unique<sfge::Button>(*getEngine()->getDefaultFont(), "+", 18, 512, 440, 24, 24);
    butincfps->setOnClick([this]() { if (fps < 500) fps++; updateFPSText(); });
}
