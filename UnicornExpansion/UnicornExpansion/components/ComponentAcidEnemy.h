#pragma once
#include "UnitComponent.h"
#include "Game.h"

// Признак отравляющего врага - отнимает здоровье на расстоянии
class ComponentAcidEnemy: public UnitComponent
{
private:
	int attack_value;
public:
	ComponentAcidEnemy(Game* game, int attack_value);
	// Сила и дальность атаки
	int getAttackValue() const;
	int getAttackDistance() const;
	virtual std::string getComponentInfo() const;
};

