#pragma once
#include "UnitComponent.h"
#include "Game.h"

// Признак атакующего врага - враг может атаковать цели вблизи
class ComponentMeleeEnemy: public UnitComponent
{
private:
	int attack_value;
	std::optional<int> targeted_unit_id;
public:
	ComponentMeleeEnemy(Game* game, int attack_value);
	// Сила и дальность атаки
	int getAttackValue() const;
	int getViewDistance() const;
	// Установить цель для атаки - координаты или юнит
	void setTargetToXY(int x, int y);
	void setTargetToUnit(int uid);
	virtual std::string getComponentInfo() const;
	virtual void update(float dt);
};

