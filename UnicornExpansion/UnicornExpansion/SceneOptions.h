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
    std::unique_ptr<sf::Text> text_caption;
    std::unique_ptr<sfge::Button> butsave;
    std::unique_ptr<sfge::Button> butcancel;
    std::unique_ptr<sfge::Checkbox> cbsound;
    std::unique_ptr<sfge::Checkbox> cbfullscreen;
    std::unique_ptr<sfge::Checkbox> cbvsync;
    std::unique_ptr<sfge::Checkbox> cbvoice;
    void saveOptions();
    std::shared_ptr<UserProfile> userprofile;
public:
    virtual void Render(sf::RenderTarget& rendertarget) override;
    virtual void Update(float dt, const sf::Vector2i& mousePos, const std::vector<sf::Event>& events) override;
    virtual void Init() override;
};
