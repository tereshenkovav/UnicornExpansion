#pragma once

#include "SfmlGameEngine/Profile.h"
#include <string>
#include <vector>
#include <format>

struct CompanyProgress {
    std::string company;
    int nextlevel;
};

class UserProfile: public sfge::Profile
{
private:
    bool voiceon = true ;
    std::vector<CompanyProgress> progress;
    std::string filename;
protected:
public:
    bool isVoiceOn() const ;
    void setVoiceOn(bool value) ;
    void loadProfile(const std::string & filename);
    void saveProfile() const;
    int getNextLevel(const std::string& company) const;
    void setLevelCompleted(const std::string& company, int level);
    static int getLevelCount(const std::string& company);
};
