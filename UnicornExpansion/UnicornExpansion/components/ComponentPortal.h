#pragma once
#include "UnitComponent.h"

// Компонент портала - задаёт признак здания, которое порождает единорогов
class ComponentPortal: public UnitComponent
{
private:
	int tek_upgrade_pos;
	int tek_increase_pos;
	int max_building_count;
public:
	ComponentPortal(Game* game);
	virtual std::vector<UnitAction> getActions() const;
	virtual bool applyAction(const UnitAction& action);
	virtual bool canApplyAction(const UnitAction& action, std::string* msgcode) const;
	virtual std::string getComponentInfo() const;
};

