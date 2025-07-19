#include "UnitComponent.h"
#include "Game.h"

UnitComponent::UnitComponent(Game* game)
{
    this->game = game;
}

void UnitComponent::setUnitID(int unit_id)
{
    this->unit_id = unit_id;
}

UnitComponent::~UnitComponent()
{
}

std::vector<UnitAction> UnitComponent::getActions() const
{
    return std::vector<UnitAction>();
}

bool UnitComponent::applyAction(const UnitAction& action)
{
    return true;
}

bool UnitComponent::canApplyAction(const UnitAction& action, std::string* msgcode) const
{
    // По умолчанию проверка только по энергии на действие
    if (game->getEnergy() < action.energy) {
        *msgcode = "Msg_NotEnoughEnergy";
        return false;
    }
    return true;
}

bool UnitComponent::doPrefixAction(const UnitAction& action)
{
    // Действие по умолчанию списывает энергию
    game->decEnergy(action.energy);
    return true;
}

std::string UnitComponent::getComponentInfo() const
{
    return std::string();
}

std::string UnitComponent::getCodePostfix() const
{
    return std::string();
}

int UnitComponent::getOrder() const
{
    return 0;
}

void UnitComponent::update(float dt)
{
}
