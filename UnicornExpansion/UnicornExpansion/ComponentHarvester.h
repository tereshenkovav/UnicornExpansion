#pragma once
#include "UnitComponent.h"

// Компонент сборщика ресурсов - данный юнит может собирать ресурсы
class ComponentHarvester : public UnitComponent
{
private:
	int harvest_rate;
	int tek_upgrade_pos;
public:
	ComponentHarvester(Game* game);
	// Скорость и дальность сбора
	int getHarvestRate() const;
	int getHarvestDistance() const;
	virtual std::vector<UnitAction> getActions() const;
	virtual bool applyAction(const UnitAction& action);
	virtual std::string getComponentInfo() const;
	virtual std::string getCodePostfix() const;
	virtual int getOrder() const;
};

