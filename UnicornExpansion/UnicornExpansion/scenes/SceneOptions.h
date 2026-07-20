#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "SfmlGameEngine/Scene.h"
#include "SfmlGameEngine/Button.h"
#include "SfmlGameEngine/Checkbox.h"
#include "UserProfile.h"

class SceneOptions: public sfge::Scene
{
private:
    sf::RectangleShape textback;
    sfge::UniqueText text_caption;
    sfge::UniqueButton butsave;
    sfge::UniqueButton butcancel;
    sfge::UniqueCheckbox cbsound;
    sfge::UniqueCheckbox cbfullscreen;
    sfge::UniqueCheckbox cbvsync;
    sfge::UniqueCheckbox cbvoice;
    sfge::UniqueCheckbox cbhidefullhealthbar;
    void saveOptions();
    std::shared_ptr<UserProfile> userprofile;
public:
    virtual void Render(sf::RenderTarget& rendertarget) override;
    virtual void Update(float dt, const sf::Vector2i& mousePos, const std::vector<sf::Event>& events) override;
    virtual void Init() override;
};
