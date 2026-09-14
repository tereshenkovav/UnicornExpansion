#include "ComponentHouse.h"
#include "Game.h"

ComponentHouse::ComponentHouse(Game* game, int value) : UnitComponent(game)
{
	unitsupport = value;
	info = 	"$Info_UnitSupport$: " + std::to_string(unitsupport);
}

int ComponentHouse::getUnitSupport() const
{
	return unitsupport;
}

std::string ComponentHouse::getComponentInfo() const
{
	return info;
}