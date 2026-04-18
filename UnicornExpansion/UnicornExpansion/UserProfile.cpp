#include "UserProfile.h"

bool UserProfile::isVoiceOn() const {
	return voiceon;
}

void UserProfile::setVoiceOn(bool value) {
	voiceon = value;
}
