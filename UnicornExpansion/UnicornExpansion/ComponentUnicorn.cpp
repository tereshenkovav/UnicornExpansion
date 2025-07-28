#include "ComponentUnicorn.h"
#include "Game.h"
#include "ComponentHarvester.h"
#include "ComponentAttacker.h"
#include "ComponentHealer.h"
#include "ComponentRadar.h"

std::vector<UnitAction> ComponentUnicorn::getActions() const
{
	std::vector<UnitAction> actions;
	if (!game->getUnitByUID(unit_id).isComponent<ComponentHarvester>())
		actions.push_back({ "make_harvester", "MakeHarvester",
			game->getConfigAction()["MakeHarvester"]["Price"].asInt(), game->getConfigAction()["MakeHarvester"]["Time"].asInt(), (UnitComponent*)this});
	if (!game->getUnitByUID(unit_id).isComponent<ComponentAttacker>())
		actions.push_back({ "make_attacker", "MakeAttacker",
			game->getConfigAction()["MakeAttacker"]["Price"].asInt(), game->getConfigAction()["MakeAttacker"]["Time"].asInt(), (UnitComponent*)this });
	if (!game->getUnitByUID(unit_id).isComponent<ComponentHealer>())
		actions.push_back({ "make_healer", "MakeHealer",
			game->getConfigAction()["MakeHealer"]["Price"].asInt(), game->getConfigAction()["MakeHealer"]["Time"].asInt(), (UnitComponent*)this });
	if (!game->getUnitByUID(unit_id).isComponent<ComponentRadar>())
		actions.push_back({ "make_radar", "MakeRadar",
			game->getConfigAction()["MakeRadar"]["Price"].asInt(), game->getConfigAction()["MakeRadar"]["Time"].asInt(), (UnitComponent*)this });
	return actions;
}

bool ComponentUnicorn::applyAction(const UnitAction& action)
{
	if (action.code == "make_harvester") {
		game->addComponentToUnitByUID(unit_id, new ComponentHarvester(game));
		game->addAudioEffect(AudioEffect::FinishResearch);
		return true;
	}
	if (action.code == "make_attacker") {
		game->addComponentToUnitByUID(unit_id, new ComponentAttacker(game));
		game->addAudioEffect(AudioEffect::FinishResearch);
		return true;
	}
	if (action.code == "make_healer") {
		game->addComponentToUnitByUID(unit_id, new ComponentHealer(game));
		game->addAudioEffect(AudioEffect::FinishResearch);
		return true;
	}
	if (action.code == "make_radar") {
		game->addComponentToUnitByUID(unit_id, new ComponentRadar(game));
		game->addAudioEffect(AudioEffect::FinishResearch);
		return true;
	}
	return false;
}

std::string ComponentUnicorn::getComponentInfo() const
{
	return "$Info_MovementSpeed$: "+std::to_string(game->getUnitByUID(unit_id).getVelocity());
}
