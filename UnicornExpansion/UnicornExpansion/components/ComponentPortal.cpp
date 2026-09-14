#include "ComponentPortal.h"
#include "UnitFactory.h"
#include "ComponentBuilding.h"
#include "ComponentMachinary.h"
#include "ComponentAcademy.h"

ComponentPortal::ComponentPortal(Game* game): UnitComponent(game)
{
	unicorn_hp = game->getConfigUnit()["Unicorn"]["InitialHP"].asInt();
	tek_upgrade_pos = 0;
	tek_increase_pos = 0;
	fastbuild = false;
	max_unicorn_count = game->getConfigComponent()["Portal"]["InitialUnicornCount"].asInt();
	max_building_count = game->getConfigComponent()["Portal"]["InitialBuildingCount"].asInt();
}

std::vector<UnitAction> ComponentPortal::getActions() const
{
	std::vector<UnitAction> actions;
	addActionIfAllowed(&actions,"build", "BuildUnicorn",fastbuild?0.67f:1.0f);
	addActionIfAllowed(&actions, "build_academy", "BuildAcademy");
	addActionIfAllowed(&actions, "build_machinary", "BuildMachinary");
	addActionIfAllowed(&actions, "upgrade_hp", "UpgradeUnicornHP", tek_upgrade_pos);
	addActionIfAllowed(&actions, "upgrade_count", "IncreaseUnicornCount", tek_increase_pos);
	if (!fastbuild) addActionIfAllowed(&actions, "fastbuild", "ResearchFastBuild");
	addActionIfAllowed(&actions, "base_shield", "SetupBaseShield");
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
			game->addTeleportationEffect((*pos).x * BLOCKW + BLOCKW / 2, (*pos).y * BLOCKH + BLOCKH / 2);
			game->addGameEvent(AudioEffect::Teleport, unit.getView());
			game->addGameEvent(AudioEffect::FinishTeleport, unit.getView());
		}
		return true;
	}
	if (action.code == "build_academy") {
		const GameUnit& unit = game->getUnitByUID(unit_id);
		auto pos = game->getFirstFreePosFor2x2Building(unit);
		if (pos) {
			UnitFactory factory(game);
			factory.addAcademy((*pos).x, (*pos).y);
			game->addGameEvent(AudioEffect::FinishBuilding, unit.getView());
		}
		return true;
	}
	if (action.code == "build_machinary") {
		const GameUnit& unit = game->getUnitByUID(unit_id);
		auto pos = game->getFirstFreePosFor2x2Building(unit);
		if (pos) {
			UnitFactory factory(game);
			factory.addMachinary((*pos).x, (*pos).y);
			game->addGameEvent(AudioEffect::FinishBuilding, unit.getView());
		}
		return true;
	}
	if (action.code == "fastbuild") {
		fastbuild = true;
		game->addGameEvent(AudioEffect::FinishResearch, game->getUnitByUID(this->unit_id).getView());
		return true;
	}
	if (action.code == "upgrade_hp") {
		unicorn_hp += game->getConfigAction()["UpgradeUnicornHP"]["Value"].asInt();
		tek_upgrade_pos++;
		game->addGameEvent(AudioEffect::FinishUpgrade, game->getUnitByUID(this->unit_id).getView());
		return true;
	}
	if (action.code == "upgrade_count") {
		max_unicorn_count += game->getConfigAction()["IncreaseUnicornCount"]["Value"].asInt();
		tek_increase_pos++;
		game->addGameEvent(AudioEffect::FinishUpgrade, game->getUnitByUID(this->unit_id).getView());
		return true;
	}
	if (action.code == "base_shield") {
		for (int i=0; i<game->getUnitCount(); i++)
			if (game->getUnit(i).isComponent<ComponentBuilding>())
				game->setShieldToUnit(game->getUnit(i).getUID(), game->getConfigAction()["SetupBaseShield"]["Amount"].asInt());
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
	if ((action.code == "build_academy")||(action.code == "build_machinary"))
		if (game->getCountUnitWithComponent<ComponentAcademy>() +
			game->getCountUnitWithComponent<ComponentMachinary>()>=max_building_count) {
			*msgcode = "Msg_TooManyBuildings";
			return false;
		}
	if (action.code == "build_academy")
		if (game->getCountUnitWithComponent<ComponentAcademy>() > 0) {
			*msgcode = "Msg_OnlyOneAcademyAllowed";
			return false;
		}
	return true;
}

std::string ComponentPortal::getComponentInfo() const
{
	std::string str = "$Info_HPNewUnicorn$: "+std::to_string(unicorn_hp)+"\n"+
		"$Info_MaxBuildingCount$: " + std::to_string(max_building_count);
	if (fastbuild) str += "\n$Info_FastBuild$";
	return str;
}

int ComponentPortal::getMaxUnicornCount() const
{
	return max_unicorn_count;
}
