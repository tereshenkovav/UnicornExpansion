#include "UnitFactory.h"
#include "ComponentResource.h"
#include "ComponentMeleeEnemy.h"
#include "ComponentPortal.h"
#include "ComponentUnicorn.h"
#include "ComponentEnemyTarget.h"
#include "ComponentEnemyLair.h"
#include "ComponentEnemy.h"
#include "ComponentMachine.h"
#include "ComponentAttacker.h"
#include "ComponentHealer.h"
#include "ComponentHarvester.h"
#include "ComponentAcademy.h"
#include "ComponentMachinary.h"
#include "ComponentBuilding.h"
#include "ComponentMultiselect.h"
#include "ComponentClearFog.h"
#include "ComponentMovable.h"

UnitFactory::UnitFactory(Game* game)
{
    this->game = game;
}

int UnitFactory::addEmptyUnit(int x, int y, int hp, const std::string & name, const std::string& code)
{
    GameUnit unit(x, y, 1, 1, name, hp, code);
    game->addUnit(unit);
    return unit.getUID();
}

int UnitFactory::addCrystal(int x, int y, int hp)
{
    GameUnit unit(x, y, 1, 1, "Crystal", hp, "crystal");
    unit.addComponent(new ComponentResource(game));
    game->addUnit(unit);
    return unit.getUID();
}

int UnitFactory::addCrystalRich(int x, int y, int hp)
{
    GameUnit unit(x, y, 1, 1, "Crystal", hp, "crystal_rich");
    unit.addComponent(new ComponentResource(game, ResourceType::Rich));
    game->addUnit(unit);
    return unit.getUID();
}

int UnitFactory::addCrystalPoor(int x, int y, int hp)
{
    GameUnit unit(x, y, 1, 1, "Crystal", hp, "crystal_poor");
    unit.addComponent(new ComponentResource(game, ResourceType::Poor));
    game->addUnit(unit);
    return unit.getUID();
}

int UnitFactory::addEnemy1(int x, int y)
{
    auto param = game->getConfigEnemy()["Enemy1"];
    GameUnit unit(x, y, 1, 1, "Bat",param["HP"].asInt(), "monster1");
    unit.setVelocity(param["Speed"].asInt());
    unit.addComponent(new ComponentMeleeEnemy(game, param["Attack"].asInt()));
    unit.addComponent(new ComponentEnemy(game));
    game->addUnit(unit);
    return unit.getUID();
}

int UnitFactory::addEnemy2(int x, int y)
{
    auto param = game->getConfigEnemy()["Enemy2"];
    GameUnit unit(x, y, 1, 1, "Octopus", param["HP"].asInt(), "monster2");
    unit.addComponent(new ComponentMeleeEnemy(game, param["Attack"].asInt()));
    unit.addComponent(new ComponentEnemy(game));
    unit.setVelocity(param["Speed"].asInt());
    game->addUnit(unit);
    return unit.getUID();
}

int UnitFactory::addEnemy3(int x, int y)
{
    auto param = game->getConfigEnemy()["Enemy3"];
    GameUnit unit(x, y, 1, 1, "Slime", param["HP"].asInt(), "monster3");
    unit.addComponent(new ComponentMeleeEnemy(game, param["Attack"].asInt()));
    unit.addComponent(new ComponentEnemy(game));
    unit.setVelocity(param["Speed"].asInt());
    game->addUnit(unit);
    return unit.getUID();
}

int UnitFactory::addLair(int x, int y, const std::string & spawnseq)
{
    auto param = game->getConfigEnemy()["Lair"];
    GameUnit unit(x, y, 2, 2, "Lair", param["HP"].asInt(), "lair");
    unit.addComponent(new ComponentEnemyLair(game,spawnseq));
    unit.addComponent(new ComponentEnemy(game));
    game->addUnit(unit);
    return unit.getUID();
}

int UnitFactory::addPortal(int x, int y)
{
    auto param = game->getConfigUnit()["Portal"];
    GameUnit unit(x, y, 2, 2, "Portal", param["HP"].asInt(), "portal");
    unit.addComponent(new ComponentPortal(game));
    unit.addComponent(new ComponentEnemyTarget(game));
    unit.addComponent(new ComponentBuilding(game));
    unit.addComponent(new ComponentClearFog(game));
    game->addUnit(unit);
    return unit.getUID();
}

int UnitFactory::addAcademy(int x, int y)
{
    auto param = game->getConfigUnit()["Academy"];
    GameUnit unit(x, y, 2, 2, "Academy", param["HP"].asInt(), "academy");
    unit.addComponent(new ComponentAcademy(game));
    unit.addComponent(new ComponentEnemyTarget(game));
    unit.addComponent(new ComponentBuilding(game));
    unit.addComponent(new ComponentClearFog(game));
    game->addUnit(unit);
    return unit.getUID();
}

int UnitFactory::addMachinary(int x, int y)
{
    auto param = game->getConfigUnit()["Machinary"];
    GameUnit unit(x, y, 2, 2, "Machinary", param["HP"].asInt(), "machinary");
    unit.addComponent(new ComponentMachinary(game));
    unit.addComponent(new ComponentEnemyTarget(game));
    unit.addComponent(new ComponentBuilding(game));
    unit.addComponent(new ComponentClearFog(game));
    game->addUnit(unit);
    return unit.getUID();
}

int UnitFactory::addUnicorn(int x, int y, int hp)
{
    auto param = game->getConfigUnit()["Unicorn"];
    GameUnit unit(x, y, 1, 1, "Unicorn", hp, "unicorn");
    unit.setVelocity(param["InitialV"].asInt());
    unit.addComponent(new ComponentUnicorn(game));
    unit.addComponent(new ComponentEnemyTarget(game));
    unit.addComponent(new ComponentMultiselect(game));
    unit.addComponent(new ComponentClearFog(game));
    unit.addComponent(new ComponentMovable(game));
    game->addUnit(unit);
    return unit.getUID();
}

int UnitFactory::addMachine(int x, int y)
{
    auto param = game->getConfigUnit()["Machine"];
    GameUnit unit(x, y, 1, 1, "Machine", param["HP"].asInt(), "machine");
    unit.setVelocity(param["V"].asInt());
    unit.addComponent(new ComponentMachine(game));
    unit.addComponent(new ComponentEnemyTarget(game));
    unit.addComponent(new ComponentMultiselect(game));
    unit.addComponent(new ComponentClearFog(game));
    unit.addComponent(new ComponentMovable(game));
    game->addUnit(unit);
    return unit.getUID();
}

int UnitFactory::addHealerTower(int x, int y)
{
    auto param = game->getConfigUnit()["HealerTower"];
    GameUnit unit(x, y, 1, 1, "HealerTower", param["HP"].asInt(), "towerhealer");
    unit.addComponent(new ComponentHealerTower(game));
    unit.addComponent(new ComponentEnemyTarget(game));
    unit.addComponent(new ComponentBuilding(game));
    unit.addComponent(new ComponentClearFog(game));
    game->addUnit(unit);
    return unit.getUID();
}

int UnitFactory::addHarvestTower(int x, int y)
{
    auto param = game->getConfigUnit()["HarvestTower"];
    GameUnit unit(x, y, 1, 1, "HarvestTower", param["HP"].asInt(), "towerharvest");
    unit.addComponent(new ComponentHarvesterTower(game));
    unit.addComponent(new ComponentEnemyTarget(game));
    unit.addComponent(new ComponentBuilding(game));
    unit.addComponent(new ComponentClearFog(game));
    game->addUnit(unit);
    return unit.getUID();
}

int UnitFactory::addAttackTower(int x, int y)
{
    auto param = game->getConfigUnit()["AttackTower"];
    GameUnit unit(x, y, 1, 1, "AttackTower", param["HP"].asInt(), "towerattack");
    unit.addComponent(new ComponentAttackerTower(game));
    unit.addComponent(new ComponentEnemyTarget(game));
    unit.addComponent(new ComponentBuilding(game));
    unit.addComponent(new ComponentClearFog(game));
    game->addUnit(unit);
    return unit.getUID();
}
