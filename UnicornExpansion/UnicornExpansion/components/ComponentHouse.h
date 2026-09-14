#pragma once
#include "UnitComponent.h"

// Компонент дома
class ComponentHouse: public UnitComponent
{
private:
	int unitsupport;
	std::string info;
public:
	ComponentHouse(Game * game, int value);
	int getUnitSupport() const;
	virtual std::string getComponentInfo() const;
};

