#pragma once

#include "SfmlGameEngine/Profile.h"

class UserProfile: public sfge::Profile
{
private:
    bool voiceon = true ;
protected:
public:
    bool isVoiceOn() const ;
    void setVoiceOn(bool value) ;
};
