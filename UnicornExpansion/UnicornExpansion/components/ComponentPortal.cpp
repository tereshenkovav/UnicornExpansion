#include "ComponentPortal.h"
#include "UnitFactory.h"
#include "ComponentBuilding.h"
#include "ComponentMachinary.h"
#include "ComponentAcademy.h"
#include "ComponentProtectable.h"

bool ComponentPortal::isAutoShield() const
{
	return tek_level>=2;
}

ComponentPortal::ComponentPortal(Game* game): UnitComponent(game)
{
	tek_level = 0;
	teleport_speed_up = 0;
	max_building_count = game->getConfigComponent()["Portal"]["InitialBuildingCount"].asInt();
}

std::vector<UnitAction> ComponentPortal::getActions() const
{
	float k = 1.0f - ((float)teleport_speed_up) / 100.f;
	std::vector<UnitAction> actions;
	addActionIfAllowed(&actions,"build", "BuildUnicorn", k);
	addActionIfAllowed(&actions, "build_academy", "BuildAcademy", k);
	addActionIfAllowed(&actions, "build_machinary", "BuildMachinary", k);
	addActionIfAllowed(&actions, "build_house", "BuildHouse", k);
	addActionIfAllowed(&actions, "build_store", "BuildStore", k);
	addActionIfAllowed(&actions, "upgrade_portal", "UpgradePortal", tek_level);
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
			int uid = factory.addUnicorn((*pos).x, (*pos).y);
			if (isAutoShield()) game->setShieldToUnit(uid, game->getConfigComponent()["Portal"]["AutoShield"].asInt());
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
	if (action.code == "build_house") {
		const GameUnit& unit = game->getUnitByUID(unit_id);
		auto pos = game->getFirstFreePosFor2x2Building(unit);
		if (pos) {
			UnitFactory factory(game);
			factory.addHouse((*pos).x, (*pos).y);
			game->addGameEvent(AudioEffect::FinishBuilding, unit.getView());
		}
		return true;
	}
	if (action.code == "build_store") {
		const GameUnit& unit = game->getUnitByUID(unit_id);
		auto pos = game->getFirstFreePosFor2x2Building(unit);
		if (pos) {
			UnitFactory factory(game);
			factory.addStore((*pos).x, (*pos).y);
			game->addGameEvent(AudioEffect::FinishBuilding, unit.getView());
		}
		return true;
	}
	if (action.code == "upgrade_portal") {
		tek_level++;
		max_building_count += game->getConfigComponent()["Portal"]["IncBuildingCount"].asInt();
		teleport_speed_up += game->getConfigComponent()["Portal"]["SpeedUpTeleport"].asInt();
		game->incUnitMaxHealth(unit_id, game->getConfigComponent()["Portal"]["IncHP"].asInt());
		game->addGameEvent(AudioEffect::FinishUpgrade, game->getUnitByUID(this->unit_id).getView());
		return true;
	}
	if (action.code == "base_shield") {
		for (int i=0; i<game->getUnitCount(); i++)
			if (game->getUnit(i).isComponent<ComponentProtectable>())
				game->setShieldToUnit(game->getUnit(i).getUID(), game->getConfigAction()["SetupBaseShield"]["Amount"].asInt());
		return true;
	}
	return false;
}

bool ComponentPortal::canApplyAction(const UnitAction& action, std::string* msgcode) const
{
	if (!UnitComponent::canApplyAction(action, msgcode)) return false;
	if (action.code == "build")
		if (game->getUnicornCount() >= game->getMaxUnicornCount()) {
			*msgcode = "Msg_NotEnoughControl";
			return false;
		}
	if ((action.code == "build_academy")||(action.code == "build_machinary")||
		(action.code == "build_house")|| (action.code == "build_store"))
		if (game->getCountUnitWithComponent<ComponentBuilding>() >=max_building_count) {
			*msgcode = "Msg_TooManyBuildings";
			return false;
		}
	if (action.code == "build_academy")
		if (game->getCountUnitWithComponent<ComponentAcademy>() > 0) {
			*msgcode = "Msg_OnlyOneAcademyAllowed";
			return false;
		}
	if (action.code == "base_shield")
		if (tek_level < 1) {
			*msgcode = "Msg_NeedSecondLevelForBaseShield";
			return false;
		}

	return true;
}

std::string ComponentPortal::getComponentInfo() const
{
	std::string str = "$Info_PortalLevel$: " + std::to_string(tek_level + 1) + "\n" +
		"$Info_MaxBuildingCount$: " + std::to_string(max_building_count);
	if (teleport_speed_up>0) str+= "\n$Info_TeleportSpeedUp$: " + std::to_string(teleport_speed_up)+"%";
	if (isAutoShield()) str += "\n$Info_AutoShield$";
	return str;
}
