#pragma once
#include <vector>

class Game;

// Максимальное число грибов в клетке
const int MAX_MUSHROOMS = 4;

struct Mushroom {
	int x;
	int y;
	int spriteid;
	int pos;
	float health;
	int index;
};

struct MushroomCell {
	std::vector<Mushroom> values;
	float nextgrown;
};

class MushroomNet
{
private:
	int width;
	int height;
	int periodgrown;
	std::vector<std::vector<MushroomCell>> net;
	Game* game;
	void growMushrooms(int x, int y);
	float genPeriod() const;
public:
	MushroomNet();
	void initByGame(Game * game);
	void setPeriodGrown(int value);
	const std::vector<Mushroom> & getMushrooms(int x, int y) const;
	int getMushroomStage(int x, int y) const;
	bool isMushroomsExist() const;
	void update(float dt);
	void setMushrooms(int x, int y, int cnt);
	void attackMushrooms(int index, float value);
};

