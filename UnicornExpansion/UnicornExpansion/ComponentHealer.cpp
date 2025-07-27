#include "ComponentHealer.h"
#include "Game.h"

ComponentHealer::ComponentHealer(Game* game) :UnitComponent(game)
{
	heal_rate = game->getConfigComponent()["Healer"]["Value"].asInt();
	tek_upgrade_pos = 0;
	locked_left = 0.0f;
}

int ComponentHealer::getHealerRate() const
{
	return heal_rate;
}

int ComponentHealer::getHealerDistance() const
{
	return game->getConfigComponent()["Healer"]["Distance"].asInt();
}

int ComponentHealer::getHealerEnergyCost() const
{
	return game->getConfigComponent()["Healer"]["Cost"].asInt();
}

std::vector<UnitAction> ComponentHealer::getActions() const
{
	std::vector<UnitAction> actions;
	if (tek_upgrade_pos < game->getConfigAction()["UpgradeHealer"]["Price"].size())
		actions.push_back({ "upgrade_healer", "UpgradeHealer",
			game->getConfigAction()["UpgradeHealer"]["Price"][tek_upgrade_pos].asInt(), game->getConfigAction()["UpgradeHealer"]["Time"][tek_upgrade_pos].asInt(), (UnitComponent*)this });

	return actions;
}

bool ComponentHealer::applyAction(const UnitAction& action)
{
	if (action.code == "upgrade_healer") {		
		heal_rate += game->getConfigComponent()["Healer"]["UpgradeStep"].asInt();
		tek_upgrade_pos++;
		return true;
	}
	return false;
}

std::string ComponentHealer::getComponentInfo() const
{
	return "$Info_HealerRate$: "+std::to_string(heal_rate);
}

std::string ComponentHealer::getCodePostfix() const
{
	return "healer";
}

int ComponentHealer::getOrder() const
{
	return 3;
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