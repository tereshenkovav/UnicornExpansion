#pragma once
#include "UnitComponent.h"

// Компонент, определяющий действие дальней атаки
//
class ComponentAttacker : public UnitComponent
{
protected:
	int attack_value;
	int attack_dist;
public:
	using UnitComponent::UnitComponent;
	// Длина и сила атаки
	virtual int getAttackValue() const ;
	virtual int getAttackDistance() const ;
	virtual std::string getComponentInfo() const;
};

class ComponentAttackerUnicorn : public ComponentAttacker
{
private:
	int tek_upgrade_pos;
public:
	ComponentAttackerUnicorn(Game* game);
	virtual std::vector<UnitAction> getActions() const;
	virtual bool applyAction(const UnitAction& action);
	virtual std::string getCodePostfix() const;
	virtual int getOrder() const;
};

class ComponentAttackerTower : public ComponentAttacker
{
private:
public:
	ComponentAttackerTower(Game* game);
};
