#include "ComponentAttacker.h"
#include "Game.h"

ComponentAttacker::ComponentAttacker(Game* game):UnitComponent(game)
{
	attack_value = game->getConfigComponent()["Attacker"]["Value"].asInt();
	tek_upgrade_pos = 0;
}

std::string ComponentAttacker::getComponentInfo() const
{
	return "$Info_AttackLevel$: "+std::to_string(attack_value);
}

int ComponentAttacker::getAttackValue() const
{
	return attack_value;
}

int ComponentAttacker::getAttackDistance() const
{
	return game->getConfigComponent()["Attacker"]["Distance"].asInt();
}

std::string ComponentAttacker::getCodePostfix() const
{
	return "attacker";
}

int ComponentAttacker::getOrder() const
{
	return 2;
}

std::vector<UnitAction> ComponentAttacker::getActions() const
{
	std::vector<UnitAction> actions;
	if (tek_upgrade_pos < game->getConfigAction()["UpgradeAttacker"]["Price"].size())
		actions.push_back({ "upgrade_attacker", "UpgradeAttacker",
			game->getConfigAction()["UpgradeAttacker"]["Price"][tek_upgrade_pos].asInt(), game->getConfigAction()["UpgradeAttacker"]["Time"][tek_upgrade_pos].asInt(), (UnitComponent*)this });
	return actions;
}

bool ComponentAttacker::applyAction(const UnitAction& action)
{
	if (action.code == "upgrade_attacker") {
		attack_value += game->getConfigComponent()["Attacker"]["UpgradeStep"].asInt();
		tek_upgrade_pos++;
		return true;
	}
	return false;
}
