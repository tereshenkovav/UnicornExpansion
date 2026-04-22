#include "ComponentRadar.h"
#include "Game.h"

std::vector<UnitAction> ComponentRadar::getActions() const
{
	std::vector<UnitAction> actions;
	addActionIfAllowed(&actions, "do_radar", "DoRadar");
	return actions;
}

bool ComponentRadar::applyAction(const UnitAction& action)
{
	if (action.code == "do_radar") {
		game->clearFogAt(game->getUnitByUID(unit_id).getXY(), game->getConfigAction()["DoRadar"]["Distance"].asInt());
		return true;
	}
	return false;
}

std::string ComponentRadar::getComponentInfo() const
{
	return "$Info_RadarDistance$: " + std::to_string(game->getConfigAction()["DoRadar"]["Distance"].asInt());
}

std::string ComponentRadar::getCodePostfix() const
{
	return "radar";
}

int ComponentRadar::getOrder() const
{
	return 4;
}
