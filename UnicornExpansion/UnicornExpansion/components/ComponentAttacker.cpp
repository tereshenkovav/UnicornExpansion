#include "ComponentAttacker.h"
#include "Game.h"

int ComponentAttacker::getAttackValue() const
{
	return attack_value;
}

int ComponentAttacker::getAttackDistance() const
{
	return attack_dist;
}

std::string ComponentAttacker::getComponentInfo() const
{
	return "$Info_AttackLevel$: " + std::to_string(attack_value);
}

ComponentAttackerUnicorn::ComponentAttackerUnicorn(Game* game):ComponentAttacker(game)
{
	attack_value = game->getConfigComponent()["Attacker"]["Value"].asInt();
	attack_dist = game->getConfigComponent()["Attacker"]["Distance"].asInt();
	tek_upgrade_pos = 0;
}

std::string ComponentAttackerUnicorn::getCodePostfix() const
{
	return "attacker";
}

int ComponentAttackerUnicorn::getOrder() const
{
	return 2;
}

std::vector<UnitAction> ComponentAttackerUnicorn::getActions() const
{
	std::vector<UnitAction> actions;
	addActionIfAllowed(&actions, "upgrade_attacker", "UpgradeAttacker", tek_upgrade_pos);
	return actions;
}

bool ComponentAttackerUnicorn::applyAction(const UnitAction& action)
{
	if (action.code == "upgrade_attacker") {
		attack_value += game->getConfigComponent()["Attacker"]["UpgradeStep"].asInt();
		tek_upgrade_pos++;
		game->addGameEvent(AudioEffect::FinishUpgrade, game->getUnitByUID(this->unit_id).getView());
		return true;
	}
	return false;
}

ComponentAttackerTower::ComponentAttackerTower(Game* game) :ComponentAttacker(game)
{
	attack_value = game->getConfigComponent()["TowerAttacker"]["Value"].asInt();
	attack_dist = game->getConfigComponent()["TowerAttacker"]["Distance"].asInt();
}
