#pragma once
#include "UnitComponent.h"

// Компонент сборщика ресурсов - данный юнит может собирать ресурсы
class ComponentHarvester : public UnitComponent
{
protected:
	int harvest_rate;
	int harvest_dist;
public:
	using UnitComponent::UnitComponent;
	// Скорость и дальность сбора
	int getHarvestRate() const;
	int getHarvestDistance() const;
	virtual std::string getComponentInfo() const;
};

class ComponentHarvesterUnicorn : public ComponentHarvester
{
private:
	int tek_upgrade_pos;
public:
	ComponentHarvesterUnicorn(Game* game);
	virtual std::vector<UnitAction> getActions() const;
	virtual bool applyAction(const UnitAction& action);
	virtual std::string getCodePostfix() const;
	virtual int getOrder() const;
};

class ComponentHarvesterTower : public ComponentHarvester
{
private:
public:
	ComponentHarvesterTower(Game* game);
};

