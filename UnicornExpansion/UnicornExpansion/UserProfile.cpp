#include "UserProfile.h"
#include <json/json.h>
#include <fstream>

bool UserProfile::isVoiceOn() const {
	return voiceon;
}

void UserProfile::setVoiceOn(bool value) {
	voiceon = value;
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
	// Далее уже специфичные для игры
	setVoiceOn(jsonProfile["options"]["voiceon"].asBool());
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
	// Далее уже специфичные для игры
	jsonProfile["options"]["voiceon"] = isVoiceOn();

	Json::StreamWriterBuilder builder;
	const std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
	std::ofstream jsonFile(filename, std::ifstream::binary);
	writer->write(jsonProfile, &jsonFile);
}
