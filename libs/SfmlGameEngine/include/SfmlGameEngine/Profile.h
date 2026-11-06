#pragma once

#include <string>

namespace sfge {

class Profile
{
private:
    bool soundon = true ;
    bool musicon = true ;
    bool fullscreen = false ;
    bool vsync = true ;
    bool stopgameonlostfocus = true;
    int fps = 60;
    std::string language = "" ;
protected:
public:
    bool isSoundOn() const ;
    bool isMusicOn() const ;
    bool isFullScreen() const ;
    bool isVSync() const ;
    int getFPS() const;
    bool isStopGameOnLostFocus() const ;
    std::string getLanguage() const;
    void setSoundOn(bool value) ;
    void setMusicOn(bool value) ;
    void setFullScreen(bool value) ;
    void setVSync(bool value) ;
    void setFPS(int value);
    void setStopGameOnLostFocus(bool value);
    void setLanguage(const std::string& value);
};

};
