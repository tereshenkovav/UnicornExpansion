#include "ComponentMeleeEnemy.h"

ComponentMeleeEnemy::ComponentMeleeEnemy(Game* game, int attack_value): UnitComponent(game)
{
	this->attack_value = attack_value;
	targeted_unit_id = std::nullopt;
	counter_attack = 0.0f;
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

void ComponentMeleeEnemy::resetAttackCounter()
{
	counter_attack = 1.0f;
}

bool ComponentMeleeEnemy::isAttackReady() const
{
	return counter_attack<=0.0f;
}

std::string ComponentMeleeEnemy::getComponentInfo() const
{
	return std::format("$Info_AttackLevel$: {}\n$Info_MovementSpeed$: {}",attack_value,game->getUnitByUID(unit_id).getVelocity());
}

void ComponentMeleeEnemy::update(float dt)
{
	if (targeted_unit_id) {
		if (game->isUnitExist(*targeted_unit_id))
			game->setSecondaryTargetToUnit(unit_id,
				game->getUnitByUID(*targeted_unit_id).getXY().x, game->getUnitByUID(*targeted_unit_id).getXY().y);
		else
			targeted_unit_id = std::nullopt;
	}
	if (counter_attack > 0.0f) counter_attack -= dt;
}
