#pragma once
#include "UnitComponent.h"

// Компонент ремонта - юнит может ремонтировать другие юниты
class ComponentHealer : public UnitComponent
{
protected:
	int heal_rate;
	int heal_dist;
	float heal_cost;
	float locked_left;
	bool isactive;
public:
	using UnitComponent::UnitComponent;
	// Скорость, дальность и расход энергии на ремонт
	int getHealerRate() const;
	int getHealerDistance() const;
	float getHealerEnergyCost() const;
	virtual std::string getComponentInfo() const;
	virtual void update(float dt);
	bool isActive() const;
	void setActive(bool value);
	void LockTemporary();
	bool isLocked() const;
};

class ComponentHealerUnicorn : public ComponentHealer
{
private:
	int tek_upgrade_pos;
public:
	ComponentHealerUnicorn(Game* game);
	virtual std::vector<UnitAction> getActions() const;
	virtual bool applyAction(const UnitAction& action);
	virtual std::string getCodePostfix() const;
	virtual int getOrder() const;
};

class ComponentHealerTower : public ComponentHealer
{
private:
public:
	ComponentHealerTower(Game* game);
};
