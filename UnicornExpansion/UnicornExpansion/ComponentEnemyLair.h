#pragma once
#include "UnitComponent.h"
#include <vector>

// ���������, ������������ ������� ������ - ����������� ��������� ������
class ComponentEnemyLair:  public UnitComponent
{
private:
	float nextmonster;
	std::vector<char> spawns;
	std::vector<char> leftspawns;
	std::vector<int> myunits; // ������ ������, ����������� � ������
public:
	// �������� ���������� � ��������� ������������������ ������ ������
	ComponentEnemyLair(Game* game, const std::string& spawnseq);
	virtual void update(float dt);
};

