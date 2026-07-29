#pragma once
#include "GameUnit.h"
#include "Game.h"

//  ласс фабрики дл€ создани€ юнитов - здесь мы создаем методы, которые создают юниты и наполн€ют их компонентами
class UnitFactory
{
private:
	Game* game;
public:
	UnitFactory(Game* game);
	int addEmptyUnit(int x, int y, int hp, const std::string& name, const std::string& code);
	int addCrystal(int x, int y, int hp);
	int addCrystalEnemyTargeted(int x, int y, int hp);
	int addCrystalRich(int x, int y, int hp);
	int addCrystalPoor(int x, int y, int hp);
	int addEnemy1(int x, int y);
	int addEnemy2(int x, int y);
	int addEnemy3(int x, int y);
	int addEnemy4(int x, int y);
	int addLair(int x, int y, const std::string& spawnseq);
	int addPortal(int x, int y);
	int addAcademy(int x, int y);
	int addMachinary(int x, int y);
	int addUnicorn(int x, int y, int hp);
	int addMachine(int x, int y);
	int addHealerTower(int x, int y);
	int addHarvestTower(int x, int y);
	int addAttackTower(int x, int y);
};

