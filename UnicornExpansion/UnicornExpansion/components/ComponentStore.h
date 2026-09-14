#pragma once
#include "UnitComponent.h"

// Компонент накопителя магии
class ComponentStore: public UnitComponent
{
private:
	int capacity;
	std::string info;
public:
	ComponentStore(Game * game, int value);
	int getCapacity() const;
	virtual std::string getComponentInfo() const;
};
