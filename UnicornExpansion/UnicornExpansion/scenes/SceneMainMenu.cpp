#include "SceneMainMenu.h"
#include "SfmlGameEngine/Engine.h"
#include "SceneStartMenu.h"

void SceneMainMenu::Render(sf::RenderTarget & rendertarget) {
    rendertarget.draw(*spr_intro);
    rendertarget.draw(*spr_title);
}

void SceneMainMenu::Update(float dt, const sf::Vector2i & mousePos, const std::vector<sf::Event>& events) {
}

void SceneMainMenu::Init() {    
    spr_intro = loadSprite("images/intro.png");
    spr_title = loadSprite("images/title.png");
    spr_title->setOrigin({ (float)(spr_title->getTexture().getSize().x / 2), 0});
    spr_title->setPosition({ 512, 60 });
        
    getEngine()->AddOverScene(std::make_shared<SceneStartMenu>());
}
