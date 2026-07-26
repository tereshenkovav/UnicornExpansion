#include "UserProfile.h"
#include <json/json.h>
#include <fstream>
#include <filesystem>

bool UserProfile::isVoiceOn() const {
	return voiceon;
}

void UserProfile::setVoiceOn(bool value) {
	voiceon = value;
}

bool UserProfile::isHideFullHealthBar() const {
	return hidefullhealthbar;
}

void UserProfile::setHideFullHealthBar(bool value) {
	hidefullhealthbar = value;
}

Difficulty UserProfile::getLastDifficulty() const
{
	return lastdifficulty;
}

void UserProfile::setLastDifficulty(Difficulty value)
{
	lastdifficulty = value;
}

void UserProfile::loadProfile(const std::string& filename)
{
	this->filename = filename;

	Json::Value jsonProfile;
	Json::CharReaderBuilder readerBuilder;
	std::ifstream jsonFile(filename, std::ifstream::binary);
	std::string errors;
	if (!Json::parseFromStream(readerBuilder, jsonFile, &jsonProfile, &errors)) return;

	// Здесь идут стандартные компоненты профиля движка, их нужно восстановить
	setSoundOn(jsonProfile["options"]["soundon"].asBool());
	setMusicOn(jsonProfile["options"]["musicon"].asBool());
	setFullScreen(jsonProfile["options"]["fullscreen"].asBool());
	setVSync(jsonProfile["options"]["vsync"].asBool());
	setLanguage(jsonProfile["options"]["language"].asString());
	// Далее уже специфичные для игры
	setVoiceOn(jsonProfile["options"]["voiceon"].asBool());
	setHideFullHealthBar(jsonProfile["options"]["hidefullhealthbar"].asBool());
	if (jsonProfile["options"]["difficulty"].isInt())
		lastdifficulty = Difficulty(jsonProfile["options"]["difficulty"].asInt());
	else
		lastdifficulty = Difficulty::Norm;

	progress.clear();
	for (int i = 0; i < jsonProfile["progress"].size(); i++)
		progress.push_back({ jsonProfile["progress"][i]["company"].asString(),
			jsonProfile["progress"][i]["nextlevel"].asInt() });
}

void UserProfile::saveProfile() const
{
	if (this->filename.length() == 0) return;

	Json::Value jsonProfile;
	// Здесь идут стандартные компоненты профиля движка, их нужно сохранить
	jsonProfile["options"]["soundon"] = isSoundOn();
	jsonProfile["options"]["musicon"] = isMusicOn();
	jsonProfile["options"]["fullscreen"] = isFullScreen();
	jsonProfile["options"]["vsync"] = isVSync();
	jsonProfile["options"]["language"] = getLanguage();
	// Далее уже специфичные для игры
	jsonProfile["options"]["voiceon"] = isVoiceOn();
	jsonProfile["options"]["hidefullhealthbar"] = isHideFullHealthBar();
	jsonProfile["options"]["difficulty"] = (int)lastdifficulty;

	Json::Value jsonProgress;
	for (auto& p : progress) {
		Json::Value company;
		company["company"] = p.company;
		company["nextlevel"] = p.nextlevel;
		jsonProgress.append(company);
	}
	jsonProfile["progress"] = jsonProgress;

	Json::StreamWriterBuilder builder;
	const std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
	std::ofstream jsonFile(filename, std::ifstream::binary);
	writer->write(jsonProfile, &jsonFile);
}

int UserProfile::getNextLevel(const std::string& company) const
{
	for (auto& p : progress)
		if (p.company == company) return p.nextlevel;
	return 0;
}

void UserProfile::setLevelCompleted(const std::string& company, int level)
{
	for (int i = 0; i < progress.size(); i++)
		if (progress[i].company == company) {
			progress[i].nextlevel = std::max(progress[i].nextlevel,level + 1);
			return;
		}
	progress.push_back({ company,level + 1 });
}
