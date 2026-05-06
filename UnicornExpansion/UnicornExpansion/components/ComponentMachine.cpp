#include "ComponentMachine.h"
#include "Game.h"
#include "UnitFactory.h"

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
	auto pos = game->getUnitByUID(unit_id).getXY();
	game->deleteUnitLater(unit_id);
	UnitFactory factory(game);
	if (action.code == "make_towerharvest") {
		factory.addHarvestTower(pos.x, pos.y);
		return true;
	}
	if (action.code == "make_towerhealer") {
		factory.addHealerTower(pos.x, pos.y);
		return true;
	}
	if (action.code == "make_towerattack") {
		factory.addAttackTower(pos.x, pos.y);
		return true;
	}
	return false;
}

std::string ComponentMachine::getComponentInfo() const
{
	return "$Info_MovementSpeed$: " + std::to_string(game->getUnitByUID(unit_id).getVelocity());
}
