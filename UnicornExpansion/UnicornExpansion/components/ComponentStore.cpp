#include "ComponentStore.h"
#include "Game.h"

ComponentStore::ComponentStore(Game* game, int value) : UnitComponent(game)
{
	capacity = value;
	info = 	"$Info_StoreCapacity$: " + std::to_string(capacity);
}

int ComponentStore::getCapacity() const
{
	return capacity;
}

std::string ComponentStore::getComponentInfo() const
{
	return info;
}