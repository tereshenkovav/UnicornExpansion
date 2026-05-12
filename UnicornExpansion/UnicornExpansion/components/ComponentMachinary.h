#pragma once
#include "UnitComponent.h"

class ComponentMachinary: public UnitComponent
{
private:
public:
	using UnitComponent::UnitComponent ;
	virtual std::vector<UnitAction> getActions() const;
	virtual bool applyAction(const UnitAction& action);
};
