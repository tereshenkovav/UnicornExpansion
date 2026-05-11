#pragma once
#include "UnitComponent.h"

class ComponentAcademy: public UnitComponent
{
private:
	bool work_in_action;
        bool magic_economy;
public:
	ComponentAcademy(Game* game);
	virtual std::vector<UnitAction> getActions() const;
	virtual bool applyAction(const UnitAction& action);
	virtual std::string getComponentInfo() const;
	bool allowWorkWhileAction() const;
	bool isMagicEconomy() const;
};

