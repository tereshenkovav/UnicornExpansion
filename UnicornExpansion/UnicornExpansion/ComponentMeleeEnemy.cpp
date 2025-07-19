#include "ComponentMeleeEnemy.h"

ComponentMeleeEnemy::ComponentMeleeEnemy(Game* game, int attack_value): UnitComponent(game)
{
	this->attack_value = attack_value;
	targeted_unit_id = std::nullopt;
}

int ComponentMeleeEnemy::getAttackValue() const
{
	return attack_value;
}

int ComponentMeleeEnemy::getViewDistance() const
{
	return game->getConfigComponent()["MeleeEnemy"]["ViewDistance"].asInt();
}

void ComponentMeleeEnemy::setTargetToXY(int x, int y)
{
	game->setTargetToUnit(unit_id, x, y);
}

void ComponentMeleeEnemy::setTargetToUnit(int uid)
{
	targeted_unit_id = uid;
}

std::string ComponentMeleeEnemy::getComponentInfo() const
{
	return "$Info_AttackLevel$: " + std::to_string(attack_value) + "\n" +
		"$Info_MovementSpeed$: " + std::to_string(game->getUnitByUID(unit_id).getVelocity());
}

void ComponentMeleeEnemy::update(float dt)
{
	if (targeted_unit_id) {
		if (game->isUnitExist(*targeted_unit_id))
			game->setTargetToUnit(unit_id,
				game->getUnitByUID(*targeted_unit_id).getXY().x, game->getUnitByUID(*targeted_unit_id).getXY().y);
		else
			targeted_unit_id = std::nullopt;
	}
}
