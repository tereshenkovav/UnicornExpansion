#pragma once
#include "UnitComponent.h"

// Компонент портала - задаёт признак здания, которое порождает единорогов
class ComponentPortal: public UnitComponent
{
private:
	int tek_level;
	int max_building_count;
	int teleport_speed_up;
public:
	ComponentPortal(Game* game);
	virtual std::vector<UnitAction> getActions() const;
	virtual bool applyAction(const UnitAction& action);
	virtual bool canApplyAction(const UnitAction& action, std::string* msgcode) const;
	virtual std::string getComponentInfo() const;
};

