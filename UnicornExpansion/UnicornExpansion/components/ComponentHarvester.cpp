#include "ComponentHarvester.h"
#include "Game.h"

ComponentHarvester::ComponentHarvester(Game* game) :UnitComponent(game)
{
	harvest_rate = game->getConfigComponent()["Harvester"]["Value"].asInt();
	tek_upgrade_pos = 0;
}

int ComponentHarvester::getHarvestRate() const
{
	return harvest_rate;
}

int ComponentHarvester::getHarvestDistance() const
{
	return game->getConfigComponent()["Harvester"]["Distance"].asInt();
}

std::vector<UnitAction> ComponentHarvester::getActions() const
{
	std::vector<UnitAction> actions;
	addActionIfAllowed(&actions, "upgrade_harvester", "UpgradeHarvester", tek_upgrade_pos);
	return actions;
}

bool ComponentHarvester::applyAction(const UnitAction& action)
{
	if (action.code == "upgrade_harvester") {
		harvest_rate += game->getConfigComponent()["Harvester"]["UpgradeStep"].asInt();
		tek_upgrade_pos++;
		game->addGameEvent(AudioEffect::FinishUpgrade, game->getUnitByUID(this->unit_id).getView());
		return true;
	}
	return false;
}

std::string ComponentHarvester::getComponentInfo() const
{
	return "$Info_HarvestRate$: "+std::to_string(harvest_rate);
}

std::string ComponentHarvester::getCodePostfix() const
{
	return "harvester";
}

int ComponentHarvester::getOrder() const
{
	return 1;
}
