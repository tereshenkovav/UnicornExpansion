#pragma once
#include "UnitComponent.h"
#include <vector>

struct Spawning {
	int time;
	char letter;
};

// Компонент, определяющий признак логова - способность порождать врагов
class ComponentEnemyLair:  public UnitComponent
{
private:
	float nextmonster;
	std::vector<Spawning> spawns;
	std::vector<Spawning> leftspawns;
	std::vector<int> myunits; // Список юнитов, порожденных в логове
public:
	// Создание компонента с указанием последовательности спавна врагов
	ComponentEnemyLair(Game* game, const std::string& spawnseq);
	virtual void update(float dt);
};

