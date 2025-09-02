#pragma once
#include "UnitComponent.h"
#include <vector>

// Компонент, определяющий признак логова - способность порождать врагов
class ComponentEnemyLair:  public UnitComponent
{
private:
	float nextmonster;
	std::vector<char> spawns;
	std::vector<char> leftspawns;
	std::vector<int> myunits; // Список юнитов, порожденных в логове
public:
	// Создание компонента с указанием последовательности спавна врагов
	ComponentEnemyLair(Game* game, const std::string& spawnseq);
	virtual void update(float dt);
};

