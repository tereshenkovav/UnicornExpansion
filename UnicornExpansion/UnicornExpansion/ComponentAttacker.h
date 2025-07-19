#pragma once
#include "UnitComponent.h"

// Компонент, определяющий действие дальней атаки
class ComponentAttacker : public UnitComponent
{
private:
	int attack_value;
	int tek_upgrade_pos;
public:
	ComponentAttacker(Game* game);
	virtual std::vector<UnitAction> getActions() const;
	virtual bool applyAction(const UnitAction& action);
	virtual std::string getComponentInfo() const;
	// Длина и сила атаки
	int getAttackValue() const;
	int getAttackDistance() const;
	virtual std::string getCodePostfix() const;
	virtual int getOrder() const;
};
