#include "ComponentDetoxer.h"
#include "Game.h"

ComponentDetoxer::ComponentDetoxer(Game* game) :UnitComponent(game)
{
	detox_value = game->getConfigComponent()["Detoxer"]["Value"].asInt();
}

std::string ComponentDetoxer::getComponentInfo() const
{
	return "$Info_DetoxLevel$: " + std::to_string(detox_value);
}

int ComponentDetoxer::getDetoxValue() const
{
	return detox_value;
}

int ComponentDetoxer::getDetoxDistance() const
{
	return game->getConfigComponent()["Detoxer"]["Distance"].asInt();
}

int ComponentDetoxer::getOrder() const
{
	return 5;
}
