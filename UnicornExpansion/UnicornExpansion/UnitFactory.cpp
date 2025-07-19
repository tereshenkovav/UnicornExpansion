#include "UnitFactory.h"
#include "ComponentResource.h"
#include "ComponentMeleeEnemy.h"
#include "ComponentPortal.h"
#include "ComponentUnicorn.h"
#include "ComponentEnemyTarget.h"
#include "ComponentEnemyLair.h"
#include "ComponentEnemy.h"

UnitFactory::UnitFactory(Game* game)
{
    this->game = game;
}

int UnitFactory::addCrystal(int x, int y, int hp)
{
    GameUnit unit(x, y, 1, 1, "Crystal", hp, "crystal");
    unit.addComponent(new ComponentResource(game));
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
    game->addUnit(unit);
    return unit.getUID();
}
