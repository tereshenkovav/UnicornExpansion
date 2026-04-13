#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include "SfmlGameEngine/Texts.h"

namespace sfge {

class Engine;

class Scene
{
private:
    Engine* engine;
    std::vector<std::unique_ptr<sf::Texture>> textures;
    std::vector<std::unique_ptr<sf::SoundBuffer>> sounds;
protected:
    Engine* getEngine() const;
    std::unique_ptr<sf::Sprite> loadSprite(const std::string& filename);
    std::unique_ptr<sf::Sound> loadSound(const std::string& filename);
    std::unique_ptr<sf::Text> loadText(int size);
    std::unique_ptr<sf::Text> loadText(const std::string & str, int size);
    std::unique_ptr<sf::Text> loadText(int size, sf::Color color);
    std::unique_ptr<sf::Text> loadText(const std::string& str, int size, sf::Color color);
    const Texts& getTexts() const;
public:
    virtual void Render(sf::RenderTarget& rendertarget);
    virtual void Update(float dt, const sf::Vector2i& mousePos, const std::vector<sf::Event>& events);
    virtual void Init();
    virtual void UnInit();

    void setEngine(Engine* engine);
};

};
