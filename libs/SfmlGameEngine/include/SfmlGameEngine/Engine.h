#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "SfmlGameEngine/Scene.h"

namespace sfge {

class Engine
{
private:
    unsigned int width;
    unsigned int height;
    bool closed;
    std::unique_ptr<sf::RenderWindow> window;
public:
    Engine(unsigned int width, unsigned int height) ;
    void Run(std::shared_ptr<Scene> scene);
    void setCaption(const std::string & str);
    void setIcon(const std::string& filename);
    void doClose();
};

};
