#include "ComponentResource.h"
#include "Game.h"

ComponentResource::ComponentResource(Game* game, ResourceType rt) : UnitComponent(game)
{
	efficient = 1.0f;
	info = "$Info_Resource$";
	if (rt == ResourceType::Poor) {
		efficient = game->getConfigComponent()["Resource"]["PoorEfficient"].asFloat();
		info += "\n$Info_PoorEfficient$";
	}
	if (rt == ResourceType::Rich) {
		efficient = game->getConfigComponent()["Resource"]["RichEfficient"].asFloat();
		info += "\n$Info_RichEfficient$";
	}
}

float ComponentResource::getResourceEfficient() const
{
	return efficient;
}

std::string ComponentResource::getComponentInfo() const
{
	return info;
}