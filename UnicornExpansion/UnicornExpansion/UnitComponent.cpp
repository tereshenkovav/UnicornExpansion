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

// Методы сильно дублируются
void UnitComponent::addActionIfAllowed(std::vector<UnitAction>* actions, const std::string & code, const std::string& caption, float fasttime) const
{
    if (!game->isActionAllowed(caption)) return;
    auto config = game->getConfigAction()[caption];
    int price = config["Price"].asInt();
    if (config["MagicEconomy"].isBool())
        if (config["MagicEconomy"].asBool())
            if (game->isMagicEconomy()) price = (int)(price * 0.8); // Пока вставим константой
    int time = (int)(config["Time"].asInt() * fasttime);
    actions->push_back({ code,caption, price, time, (UnitComponent*)this });
}

// Методы сильно дублируются
void UnitComponent::addActionIfAllowed(std::vector<UnitAction>* actions, const std::string& code, const std::string& caption, int idx, float fasttime) const
{
    if (!game->isActionAllowed(caption)) return;
    auto config = game->getConfigAction()[caption];
    if (idx < config["Price"].size()) {
        int price = config["Price"][idx].asInt();
        if (config["MagicEconomy"].isBool())
            if (config["MagicEconomy"].asBool())
                if (game->isMagicEconomy()) price = (int)(price * 0.8); // Пока вставим константой
        int time = (int)(config["Time"][idx].asInt() * fasttime);
        actions->push_back({ code,caption, price, time, (UnitComponent*)this });
    }
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

bool UnitComponent::undoAction(const UnitAction& action)
{
    // Действие при отмене по умолчанию восстанавливает энергию
    game->incEnergy(action.energy);
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
