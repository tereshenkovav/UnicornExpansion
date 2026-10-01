#include "ComponentAcidEnemy.h"

ComponentAcidEnemy::ComponentAcidEnemy(Game* game, int attack_value): UnitComponent(game)
{
	this->attack_value = attack_value;
}

int ComponentAcidEnemy::getAttackValue() const
{
	return attack_value;
}

int ComponentAcidEnemy::getAttackDistance() const
{
	return game->getConfigComponent()["AcidEnemy"]["AttackDistance"].asInt();
}

std::string ComponentAcidEnemy::getComponentInfo() const
{
	return std::format("$Info_AttackLevel$: {}",attack_value);
}
