#include "ComponentMachine.h"
#include "Game.h"

std::vector<UnitAction> ComponentMachine::getActions() const
{
	std::vector<UnitAction> actions;
	addActionIfAllowed(&actions, "make_towerharvest", "MakeTowerHarvest");
	addActionIfAllowed(&actions, "make_towerattack", "MakeTowerAttack");
	addActionIfAllowed(&actions, "make_towerhealer", "MakeTowerHealer");
	return actions;
}

bool ComponentMachine::applyAction(const UnitAction& action)
{
	if (action.code == "do_radar") {
		game->clearFogAt(game->getUnitByUID(unit_id).getXY(), game->getConfigAction()["DoRadar"]["Distance"].asInt());
		return true;
	}
	return false;
}

std::string ComponentMachine::getComponentInfo() const
{
	return "$Info_MovementSpeed$: " + std::to_string(game->getUnitByUID(unit_id).getVelocity());
}
