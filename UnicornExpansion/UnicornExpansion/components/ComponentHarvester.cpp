#include "ComponentHarvester.h"
#include "Game.h"

int ComponentHarvester::getHarvestRate() const
{
	return harvest_rate;
}

int ComponentHarvester::getHarvestDistance() const
{
	return harvest_dist;
}

std::string ComponentHarvester::getComponentInfo() const
{
	return "$Info_HarvestRate$: " + std::to_string(harvest_rate);
}

ComponentHarvesterUnicorn::ComponentHarvesterUnicorn(Game* game):ComponentHarvester(game)
{
	harvest_rate = game->getConfigComponent()["Harvester"]["Value"].asInt();
	harvest_dist = game->getConfigComponent()["Harvester"]["Distance"].asInt();
	tek_upgrade_pos = 0;
}

std::vector<UnitAction> ComponentHarvesterUnicorn::getActions() const
{
	std::vector<UnitAction> actions;
	addActionIfAllowed(&actions, "upgrade_harvester", "UpgradeHarvester", tek_upgrade_pos);
	return actions;
}

bool ComponentHarvesterUnicorn::applyAction(const UnitAction& action)
{
	if (action.code == "upgrade_harvester") {
		harvest_rate += game->getConfigComponent()["Harvester"]["UpgradeStep"].asInt();
		tek_upgrade_pos++;
		game->addGameEvent(AudioEffect::FinishUpgrade, game->getUnitByUID(this->unit_id).getView());
		return true;
	}
	return false;
}

std::string ComponentHarvesterUnicorn::getCodePostfix() const
{
	return "harvester";
}

int ComponentHarvesterUnicorn::getOrder() const
{
	return 1;
}

ComponentHarvesterTower::ComponentHarvesterTower(Game* game) :ComponentHarvester(game)
{
	harvest_rate = game->getConfigComponent()["TowerHarvester"]["Value"].asInt();
	harvest_dist = game->getConfigComponent()["TowerHarvester"]["Distance"].asInt();
}
