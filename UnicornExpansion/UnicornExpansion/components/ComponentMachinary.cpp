#include "ComponentMachinary.h"
#include "UnitFactory.h"

std::vector<UnitAction> ComponentMachinary::getActions() const
{
	std::vector<UnitAction> actions;
	addActionIfAllowed(&actions,"construct", "ConstructMachine");
	return actions;
}

bool ComponentMachinary::applyAction(const UnitAction& action)
{
	if (action.code == "construct") {
		const GameUnit & unit = game->getUnitByUID(unit_id);
		auto pos = game->getFirstFreePosNear(unit);
		if (pos) {
			UnitFactory factory(game);
			factory.addMachine((*pos).x, (*pos).y);
			game->addGameEvent(AudioEffect::FinishConstruct, unit.getView());
		}
		return true;
	}
	return false;
}
