#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "SfmlGameEngine/Scene.h"
#include "SfmlGameEngine/Texts.h"
#include "SfmlGameEngine/Colors.h"
#include <map>

namespace sfge {

class Engine
{
private:
    unsigned int width;
    unsigned int height;
    bool closed;
    bool exitscene;
    std::unique_ptr<sf::RenderWindow> window;
    std::shared_ptr<Scene> nextscene;
    std::shared_ptr<Scene> overscene;
    std::shared_ptr<sf::Font> defaultfont;
    std::vector<std::unique_ptr<sf::Texture>> textures;
    std::shared_ptr<sf::Sprite> defaultcursor;
    std::weak_ptr<sf::Sprite> currentcursor;
    std::map<int, std::shared_ptr<sf::Sprite>> cursors;
    Texts texts;
    Colors colors;
public:
    Engine(unsigned int width, unsigned int height) ;
    void loadDefaultFont(const std::string& filename);
    std::shared_ptr<sf::Font> getDefaultFont() const;
    void loadDefaultCursor(const std::string& filename);
    void addCursor(int code, const std::string& filename);
    void setCursor(int code);
    void Run(std::shared_ptr<Scene> scene);
    void SwitchToScene(std::shared_ptr<Scene> scene);
    void AddOverScene(std::shared_ptr<Scene> scene);
    void setCaption(const std::string & str);
    void setIcon(const std::string& filename);
    void loadTexts(const std::string& filename);
    const Texts& getTexts() const;
    void loadColors(const std::string& filename);
    const Colors& getColors() const;
    sf::Vector2f getWorldPosByView(const sf::View& view, sf::Vector2i pos);
    void doClose();
    void doExitScene();
};

};
