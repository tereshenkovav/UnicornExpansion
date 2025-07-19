#pragma once
#include "UnitComponent.h"

// Компонент ресурса - его можно добывать
class ComponentResource: public UnitComponent
{
public:
	using UnitComponent::UnitComponent;
	virtual std::string getComponentInfo() const;
};

