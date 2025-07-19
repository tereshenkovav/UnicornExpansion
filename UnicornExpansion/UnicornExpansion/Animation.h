#pragma once

#include <SFML/Graphics.hpp>

// ����� �������� - ��������� ������� ����� ��������, ������� ����������� �� ������ update. � ��������� ��� ������� ������
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
    // ����������� ��������, ����� ������ �������, ����� ������ � ������� ��������
    Animation(const std::string & filename, int w, int h, int framecount, int fps) ;
    // ���������� ��������
    void update(double dt) ;
    int getFrame() const;

    // ���������� ���������
    void play() ;
    void playOneTime() ;
    void stop() ;
    bool isPlayed() ;
};

