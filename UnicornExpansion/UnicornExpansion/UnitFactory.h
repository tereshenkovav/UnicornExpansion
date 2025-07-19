#pragma once
#include "GameUnit.h"
#include "Game.h"

// ����� ������� ��� �������� ������ - ����� �� ������� ������, ������� ������� ����� � ��������� �� ������������
class UnitFactory
{
private:
	Game* game;
public:
	UnitFactory(Game* game);
	int addCrystal(int x, int y, int hp);
	int addEnemy1(int x, int y);
	int addEnemy2(int x, int y);
	int addEnemy3(int x, int y);
	int addLair(int x, int y, const std::string& spawnseq);
	int addPortal(int x, int y);
	int addUnicorn(int x, int y, int hp);
};

