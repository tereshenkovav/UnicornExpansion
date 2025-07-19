#include "ComponentPortal.h"
#include "UnitFactory.h"

ComponentPortal::ComponentPortal(Game* game): UnitComponent(game)
{
	unicorn_hp = game->getConfigUnit()["Unicorn"]["InitialHP"].asInt();
	tek_upgrade_pos = 0;
	tek_increase_pos = 0;
	max_unicorn_count = game->getConfigComponent()["Portal"]["InitialUnicornCount"].asInt();
}

std::vector<UnitAction> ComponentPortal::getActions() const
{
	std::vector<UnitAction> actions;
	actions.push_back({ "build", "BuildUnicorn",
		game->getConfigAction()["BuildUnicorn"]["Price"].asInt(), game->getConfigAction()["BuildUnicorn"]["Time"].asInt(), (UnitComponent*)this});
	if (tek_upgrade_pos < game->getConfigAction()["UpgradeUnicornHP"]["Price"].size())
		actions.push_back({ "upgrade_hp", "UpgradeUnicornHP",
			game->getConfigAction()["UpgradeUnicornHP"]["Price"][tek_upgrade_pos].asInt(), game->getConfigAction()["UpgradeUnicornHP"]["Time"][tek_upgrade_pos].asInt(), (UnitComponent*)this });

	if (tek_increase_pos < game->getConfigAction()["IncreaseUnicornCount"]["Price"].size())
		actions.push_back({ "upgrade_count", "IncreaseUnicornsCount",
			game->getConfigAction()["IncreaseUnicornCount"]["Price"][tek_upgrade_pos].asInt(), game->getConfigAction()["IncreaseUnicornCount"]["Time"][tek_upgrade_pos].asInt(), (UnitComponent*)this });

	return actions;
}

bool ComponentPortal::applyAction(const UnitAction& action)
{
	if (action.code == "build") {
		const GameUnit & unit = game->getUnitByUID(unit_id);
		auto pos = game->getFirstFreePosNear(unit);
		if (pos) {
			UnitFactory factory(game);
			factory.addUnicorn((*pos).x, (*pos).y, unicorn_hp);
		}
		return true;
	}
	if (action.code == "upgrade_hp") {
		unicorn_hp += game->getConfigAction()["UpgradeUnicornHP"]["Value"].asInt();
		tek_upgrade_pos++;
		return true;
	}
	if (action.code == "upgrade_count") {
		max_unicorn_count += game->getConfigAction()["IncreaseUnicornCount"]["Value"].asInt();
		tek_increase_pos++;
		return true;
	}
	return false;
}

bool ComponentPortal::canApplyAction(const UnitAction& action, std::string* msgcode) const
{
	if (!UnitComponent::canApplyAction(action, msgcode)) return false;
	if (action.code == "build")
		if (game->getUnicornCount() >= max_unicorn_count) {
			*msgcode = "Msg_NotEnoughControl";
			return false;
		}
	return true;
}

std::string ComponentPortal::getComponentInfo() const
{
	return "$Info_HPNewUnicorn$: "+std::to_string(unicorn_hp);
}

int ComponentPortal::getMaxUnicornCount() const
{
	return max_unicorn_count;
}
