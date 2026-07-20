#pragma once

#include "SfmlGameEngine/Profile.h"
#include <string>
#include <vector>
#include <format>
#include "Game.h"

struct CompanyProgress {
    std::string company;
    int nextlevel;
};

class UserProfile: public sfge::Profile
{
private:
    bool voiceon = true ;
    bool hidefullhealthbar = true;
    std::vector<CompanyProgress> progress;
    Difficulty lastdifficulty = Difficulty::Norm;
    std::string filename;
protected:
public:
    bool isVoiceOn() const ;
    void setVoiceOn(bool value) ;
    bool isHideFullHealthBar() const;
    void setHideFullHealthBar(bool value);
    Difficulty getLastDifficulty() const;
    void setLastDifficulty(Difficulty value);
    void loadProfile(const std::string & filename);
    void saveProfile() const;
    int getNextLevel(const std::string& company) const;
    void setLevelCompleted(const std::string& company, int level);
    static int getLevelCount(const std::string& company);
};
