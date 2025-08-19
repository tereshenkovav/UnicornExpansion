#pragma once

#include "Game.h"
#include <map>

class SeedStore
{
private:
	static std::map<int, std::map<LaserType,float>> seeds;
public:
	static float getSeedByUIDAndLaserType(int uid, LaserType lasertype);
};

