#pragma once
#include "UnitComponent.h"
#include "Game.h"

// ������� ���������� ����� - ���� ����� ��������� ���� ������
class ComponentMeleeEnemy: public UnitComponent
{
private:
	int attack_value;
	std::optional<int> targeted_unit_id;
public:
	ComponentMeleeEnemy(Game* game, int attack_value);
	// ���� � ��������� �����
	int getAttackValue() const;
	int getViewDistance() const;
	// ���������� ���� ��� ����� - ���������� ��� ����
	void setTargetToXY(int x, int y);
	void setTargetToUnit(int uid);
	virtual std::string getComponentInfo() const;
	virtual void update(float dt);
};

