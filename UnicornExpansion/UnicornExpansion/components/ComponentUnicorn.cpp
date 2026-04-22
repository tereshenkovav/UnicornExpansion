#include "ComponentUnicorn.h"
#include "Game.h"
#include "ComponentHarvester.h"
#include "ComponentAttacker.h"
#include "ComponentHealer.h"
#include "ComponentRadar.h"
#include "ComponentShield.h"
#include "ComponentDetoxer.h"

std::vector<UnitAction> ComponentUnicorn::getActions() const
{
	std::vector<UnitAction> actions;
	const GameUnit& unit = game->getUnitByUID(unit_id);
	if (!unit.isComponent<ComponentHarvester>()) addActionIfAllowed(&actions,"make_harvester", "MakeHarvester");
	if (!unit.isComponent<ComponentAttacker>()) addActionIfAllowed(&actions, "make_attacker", "MakeAttacker");
	if (!unit.isComponent<ComponentHealer>()) addActionIfAllowed(&actions, "make_healer", "MakeHealer");
	if (!unit.isComponent<ComponentRadar>()) addActionIfAllowed(&actions, "make_radar", "MakeRadar");
	if (!unit.isComponent<ComponentShield>()) addActionIfAllowed(&actions, "make_shield", "MakeShield");
	if (!unit.isComponent<ComponentDetoxer>()) addActionIfAllowed(&actions, "make_detoxer", "MakeDetoxer");
	return actions;
}

bool ComponentUnicorn::applyAction(const UnitAction& action)
{
	if (action.code == "make_harvester") {
		game->addComponentToUnitByUID(unit_id, new ComponentHarvester(game));
		game->addGameEvent(AudioEffect::FinishResearch, game->getUnitByUID(this->unit_id).getView());
		return true;
	}
	if (action.code == "make_attacker") {
		game->addComponentToUnitByUID(unit_id, new ComponentAttacker(game));
		game->addGameEvent(AudioEffect::FinishResearch, game->getUnitByUID(this->unit_id).getView());
		return true;
	}
	if (action.code == "make_healer") {
		game->addComponentToUnitByUID(unit_id, new ComponentHealer(game));
		game->addGameEvent(AudioEffect::FinishResearch, game->getUnitByUID(this->unit_id).getView());
		return true;
	}
	if (action.code == "make_radar") {
		game->addComponentToUnitByUID(unit_id, new ComponentRadar(game));
		game->addGameEvent(AudioEffect::FinishResearch, game->getUnitByUID(this->unit_id).getView());
		return true;
	}
	if (action.code == "make_shield") {
		game->addComponentToUnitByUID(unit_id, new ComponentShield(game));
		game->addGameEvent(AudioEffect::FinishResearch, game->getUnitByUID(this->unit_id).getView());
		return true;
	}
	if (action.code == "make_detoxer") {
		game->addComponentToUnitByUID(unit_id, new ComponentDetoxer(game));
		game->addGameEvent(AudioEffect::FinishResearch, game->getUnitByUID(this->unit_id).getView());
		return true;
	}
	return false;
}

std::string ComponentUnicorn::getComponentInfo() const
{
	return "$Info_MovementSpeed$: "+std::to_string(game->getUnitByUID(unit_id).getVelocity());
}
