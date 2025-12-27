#pragma once
#include "UnitComponent.h"

// Компонент щита - устанавливает на себя щит, поглощающий урон
class ComponentShield : public UnitComponent
{
private:
public:
	using UnitComponent::UnitComponent;
	virtual std::vector<UnitAction> getActions() const;
	virtual bool applyAction(const UnitAction& action);
	virtual std::string getCodePostfix() const;
	virtual int getOrder() const;
};
