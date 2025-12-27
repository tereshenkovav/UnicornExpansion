#include "ComponentShield.h"
#include "Game.h"

std::vector<UnitAction> ComponentShield::getActions() const
{
	std::vector<UnitAction> actions;
	actions.push_back({ "do_shield", "DoShield",
		game->getConfigAction()["DoShield"]["Price"].asInt(), game->getConfigAction()["DoShield"]["Time"].asInt(), (UnitComponent*)this});
	return actions;
}

bool ComponentShield::applyAction(const UnitAction& action)
{
	if (action.code == "do_shield") {
		game->setShieldToUnit(this->unit_id, game->getConfigAction()["DoShield"]["Amount"].asInt());
		return true;
	}
	return false;
}

std::string ComponentShield::getCodePostfix() const
{
	return "shield";
}

int ComponentShield::getOrder() const
{
	return 5;
}
