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
	const GameUnit& unit = game->getUnitByUID(unit_id);
	game->deleteUnitLater(unit_id);
	UnitFactory factory(game);
	if (action.code == "make_towerharvest") {
		factory.addHarvestTower(unit.getXY().x, unit.getXY().y);
		game->addGameEvent(AudioEffect::FinishBuilding, unit.getView());
		return true;
	}
	if (action.code == "make_towerhealer") {
		factory.addHealerTower(unit.getXY().x, unit.getXY().y);
		game->addGameEvent(AudioEffect::FinishBuilding, unit.getView());
		return true;
	}
	if (action.code == "make_towerattack") {
		factory.addAttackTower(unit.getXY().x, unit.getXY().y);
		game->addGameEvent(AudioEffect::FinishBuilding, unit.getView());
		return true;
	}
	return false;
}

std::string ComponentMachine::getComponentInfo() const
{
	return "$Info_MovementSpeed$: " + std::to_string(game->getUnitByUID(unit_id).getVelocity());
}
