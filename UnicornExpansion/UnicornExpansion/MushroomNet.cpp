#include "MushroomNet.h"
#include "Game.h"

const int MUSHROOM_TYPES = 6;
const int SHIFT = 8;
const int PERIODGROWN = 20.0f;

float genPeriod() {
	return PERIODGROWN + 0.05f * (rand() % (10 * PERIODGROWN));
}

void MushroomNet::growMushrooms(int x, int y)
{
	if (net[x][y].values.size() >= MAX_MUSHROOMS) return;

	std::vector<int> allposes{ 0, 1, 2, 3 };
	for (auto m: net[x][y].values)
		allposes.erase(std::remove(allposes.begin(), allposes.end(), m.pos), allposes.end());
	int n = allposes[rand() % allposes.size()];
	Mushroom m{ 32 * (n / 2) + (rand() % (2 * SHIFT + 1)) - SHIFT, 
		32 * (n % 2) + (rand() % (2*SHIFT+1)) - SHIFT, rand() % MUSHROOM_TYPES, n };
	net[x][y].values.push_back(m);
}

MushroomNet::MushroomNet()
{
}

void MushroomNet::initByGame(Game* game)
{
	this->game = game;
	this->width = game->getWidth();
	this->height = game->getHeight();

	net.resize(width);
	for (int i = 0; i < width; i++) {
		net[i].resize(height);
		for (int j = 0; j < height; j++)
			net[i][j].nextgrown = genPeriod();
	}
}

const std::vector<Mushroom> & MushroomNet::getMushrooms(int x, int y) const
{
	return net[x][y].values;
}

int MushroomNet::getMushroomStage(int x, int y) const
{
	return net[x][y].values.size();
}

void MushroomNet::update(float dt)
{
	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++) {
			net[i][j].nextgrown -= dt;
			if (net[i][j].nextgrown < 0.0f) {
				net[i][j].nextgrown = genPeriod();
				if (!Game::canWalkOnTerrain(game->getMap(i, j))) continue ;
				if (net[i][j].values.size() >= MAX_MUSHROOMS) continue;
				// ƒл€ пустой €чейки условие роста - соседние клетки с плотностью грибов не менее 2
				if (net[i][j].values.size() == 0) {
					bool isnear = false;
					if (i > 0) isnear = isnear || (net[i - 1][j].values.size() >= 2);
					if (i < width - 1) isnear = isnear || (net[i + 1][j].values.size() >= 2);
					if (j > 0) isnear = isnear || (net[i][j - 1].values.size() >= 2);
					if (j < height - 1) isnear = isnear || (net[i][j + 1].values.size() >= 2);
					if (isnear) growMushrooms(i, j);
				}
				else // ƒл€ уже с грибами - просто растЄм
					growMushrooms(i, j);
			}
		}
}

void MushroomNet::setMushrooms(int x, int y, int cnt)
{
	net[x][y].values.clear();
	for (int i = 0; i < cnt; i++)
		growMushrooms(x, y);
}
