#pragma once

#include "SfmlGameEngine/Profile.h"
#include <string>

class UserProfile: public sfge::Profile
{
private:
    bool voiceon = true ;
    std::string filename;
protected:
public:
    bool isVoiceOn() const ;
    void setVoiceOn(bool value) ;
    void loadProfile(const std::string & filename);
    void saveProfile() const;
};
