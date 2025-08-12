#pragma once
#include <vector>

class Game;

const int MAX_MUSHROOMS = 4;

struct Mushroom {
	int x;
	int y;
	int id;
	int pos;
	float health;
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
	std::vector<std::vector<MushroomCell>> net;
	Game* game;
	void growMushrooms(int x, int y);
public:
	MushroomNet();
	void initByGame(Game * game);
	const std::vector<Mushroom> & getMushrooms(int x, int y) const;
	int getMushroomStage(int x, int y) const;
	bool isMushroomsExist() const;
	void update(float dt);
	void setMushrooms(int x, int y, int cnt);
	void attackMushrooms(int x, int y, float value);
};

