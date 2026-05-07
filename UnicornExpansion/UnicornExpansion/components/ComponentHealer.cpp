#include "ComponentHealer.h"
#include "Game.h"

int ComponentHealer::getHealerRate() const
{
	return heal_rate;
}

int ComponentHealer::getHealerDistance() const
{
	return heal_dist;
}

int ComponentHealer::getHealerEnergyCost() const
{
	return heal_cost;
}

std::string ComponentHealer::getComponentInfo() const
{
	return "$Info_HealerRate$: " + std::to_string(heal_rate);
}

void ComponentHealer::update(float dt)
{
	if (locked_left > 0.0f) locked_left -= dt;
}

bool ComponentHealer::isActive() const
{
	return isactive;
}

void ComponentHealer::setActive(bool value)
{
	isactive = value;
}

void ComponentHealer::LockTemporary()
{
	isactive = false;
	locked_left = 0.5f;
}

bool ComponentHealer::isLocked() const
{
	return locked_left > 0.0f;
}

ComponentHealerUnicorn::ComponentHealerUnicorn(Game* game) :ComponentHealer(game)
{
	heal_rate = game->getConfigComponent()["Healer"]["Value"].asInt();
	heal_dist = game->getConfigComponent()["Healer"]["Distance"].asInt();
	heal_cost = game->getConfigComponent()["Healer"]["Cost"].asInt();
	tek_upgrade_pos = 0;
	locked_left = 0.0f;
}

std::vector<UnitAction> ComponentHealerUnicorn::getActions() const
{
	std::vector<UnitAction> actions;
	addActionIfAllowed(&actions, "upgrade_healer", "UpgradeHealer", tek_upgrade_pos);
	return actions;
}

bool ComponentHealerUnicorn::applyAction(const UnitAction& action)
{
	if (action.code == "upgrade_healer") {
		heal_rate += game->getConfigComponent()["Healer"]["UpgradeStep"].asInt();
		tek_upgrade_pos++;
		game->addGameEvent(AudioEffect::FinishUpgrade, game->getUnitByUID(this->unit_id).getView());
		return true;
	}
	return false;
}

std::string ComponentHealerUnicorn::getCodePostfix() const
{
	return "healer";
}

int ComponentHealerUnicorn::getOrder() const
{
	return 3;
}

ComponentHealerTower::ComponentHealerTower(Game* game) :ComponentHealer(game)
{
	heal_rate = game->getConfigComponent()["TowerHealer"]["Value"].asInt();
	heal_dist = game->getConfigComponent()["TowerHealer"]["Distance"].asInt();
	heal_cost = game->getConfigComponent()["TowerHealer"]["Cost"].asInt();
	locked_left = 0.0f;
}
