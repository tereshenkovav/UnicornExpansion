#pragma once

#include "UnitComponent.h"

// Компонент единорога - это подвижный собственный юнит
class ComponentUnicorn : public UnitComponent
{
private:
public:
	using UnitComponent::UnitComponent;
	virtual std::vector<UnitAction> getActions() const;
	virtual bool applyAction(const UnitAction& action);
	virtual std::string getComponentInfo() const;
};

