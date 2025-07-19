#pragma once

#include <SFML/Graphics.hpp>

//  ласс анимации - позвол€ет создать серию спрайтов, котора€ анимируетс€ по методу update. ¬ остальном это обычный спрайт
class Animation : public sf::Sprite
{
private:
    sf::Texture * frames ;
    int count ;
    float fps ;
    float tekt ;
    bool isplayed ;
    bool onceflag ;

    sf::Texture deftex;

public:
    //  онструктор анимации, задаЄт размер спрайта, число кадров и частоту анимации
    Animation(const std::string & filename, int w, int h, int framecount, int fps) ;
    // ќбновление анимации
    void update(double dt) ;
    int getFrame() const;

    // ”правление анимацией
    void play() ;
    void playOneTime() ;
    void stop() ;
    bool isPlayed() ;
};

