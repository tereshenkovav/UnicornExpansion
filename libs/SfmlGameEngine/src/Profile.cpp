#include "SfmlGameEngine/Profile.h"

namespace sfge {

bool Profile::isSoundOn() const {
	return soundon;
}

bool Profile::isMusicOn() const {
	return musicon;
}

bool Profile::isFullScreen() const {
	return fullscreen;
}

bool Profile::isVSync() const {
	return vsync;
}

void Profile::setSoundOn(bool value) {
	soundon = value;
}

void Profile::setMusicOn(bool value) {
	musicon = value;
}

void Profile::setFullScreen(bool value) {
	fullscreen = value;
}

void Profile::setVSync(bool value) {
	vsync = value;
}

}
