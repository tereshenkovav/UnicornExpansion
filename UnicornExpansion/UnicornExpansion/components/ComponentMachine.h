#pragma once
#include "UnitComponent.h"

// Компонент машины
class ComponentMachine : public UnitComponent
{
private:
public:
	using UnitComponent::UnitComponent;
	virtual std::vector<UnitAction> getActions() const;
	virtual bool applyAction(const UnitAction& action);
	virtual std::string getComponentInfo() const;
};


