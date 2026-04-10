#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

namespace sfge {

class Engine;

class Scene
{
private:
    Engine* engine;
    std::vector<std::unique_ptr<sf::Texture>> textures;
protected:
    Engine* getEngine() const;
    std::unique_ptr<sf::Sprite> loadSprite(const std::string& filename);
public:
    virtual void Render(sf::RenderTarget& rendertarget);
    virtual void Update(float dt, const sf::Vector2i& mousePos, const std::vector<sf::Event>& events);
    virtual void Init();
    virtual void UnInit();

    void setEngine(Engine* engine);
};

};
