#include "ComponentEnemyLair.h"
#include "GameUnit.h"
#include "Game.h"
#include "UnitFactory.h"
#include "ComponentMeleeEnemy.h"
#include "ComponentPortal.h"

ComponentEnemyLair::ComponentEnemyLair(Game* game, const std::string & spawnseq):UnitComponent(game)
{
	nextmonster = game->getConfigComponent()["Lair"]["SpawnInterval"].asInt();
	for (size_t i = 0; i < spawnseq.size(); i++)
		spawns.push_back(spawnseq[i]);
	leftspawns = spawns;
}

void ComponentEnemyLair::update(float dt)
{
	nextmonster -= dt;
	if (nextmonster <= 0) {
		nextmonster = game->getConfigComponent()["Lair"]["SpawnInterval"].asInt();
		const GameUnit& unit = game->getUnitByUID(unit_id);
		auto pos = game->getFirstFreePosNear(unit);
		if (pos) {
			UnitFactory factory(game);
			// Порождаем юнитов по паттерну - каждая буква это свой тип юнита
			int rnd = rand() % leftspawns.size();
			if (leftspawns[rnd] == 'a') myunits.push_back(factory.addEnemy1((*pos).x, (*pos).y));
			if (leftspawns[rnd] == 'b') myunits.push_back(factory.addEnemy2((*pos).x, (*pos).y));
			if (leftspawns[rnd] == 'c') myunits.push_back(factory.addEnemy3((*pos).x, (*pos).y));

			leftspawns.erase(leftspawns.begin() + rnd);
			// Код отправки в атаку на ангар
			if (leftspawns.size() == 0) {
				leftspawns = spawns;

				std::optional<int> portal_id = std::nullopt;
				for (int i = 0; i < game->getUnitCount(); i++)
					if (game->getUnit(i).isComponent<ComponentPortal>()) portal_id = game->getUnit(i).getUID();

				if (portal_id) {
					for (size_t i = 0; i < myunits.size(); i++)
						if (game->isUnitExist(myunits[i]))
							if (auto* enemymelee = game->getUnitByUID(myunits[i]).getComponent<ComponentMeleeEnemy>())
								enemymelee->setTargetToUnit(*portal_id);
					myunits.clear();
				}
			}
		}
	}
}

