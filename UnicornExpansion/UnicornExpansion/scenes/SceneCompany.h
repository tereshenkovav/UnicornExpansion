#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "SfmlGameEngine/Scene.h"
#include "SfmlGameEngine/Button.h"
#include "SfmlGameEngine/CheckboxGroup.h"
#include "UserProfile.h"
#include "Game.h"

class SceneCompany: public sfge::Scene
{
private:
    sfge::UniqueSprite spr_intro;
    sf::RectangleShape textback;
    sf::RectangleShape textback1;
    sfge::UniqueText text_caption;
    sfge::UniqueText text_info;
    sfge::UniqueButton butcancel;
    sfge::UniqueCheckboxGroup cbgroup;
    std::vector<sfge::UniqueButton> buttons;
    std::shared_ptr<UserProfile> userprofile;
    std::string company;
    std::vector<std::string> companylevels;
    std::string companyinfo;
public:
    SceneCompany(const std::string& company);
    virtual void Render(sf::RenderTarget& rendertarget) override;
    virtual void Update(float dt, const sf::Vector2i& mousePos, const std::vector<sf::Event>& events) override;
    virtual void Init() override;
};
