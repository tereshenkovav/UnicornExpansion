#include "ComponentAcademy.h"
#include "UnitFactory.h"

ComponentAcademy::ComponentAcademy(Game* game): UnitComponent(game)
{
	work_in_action = false;
    magic_economy = false;
}

std::vector<UnitAction> ComponentAcademy::getActions() const
{
	std::vector<UnitAction> actions;
	if (!work_in_action) addActionIfAllowed(&actions, "research_workinaction", "ResearchWorkInAction");
	if (!magic_economy) addActionIfAllowed(&actions, "research_magiceconomy", "ResearchMagicEconomy");
	return actions;
}

bool ComponentAcademy::applyAction(const UnitAction& action)
{
	if (action.code == "research_workinaction") {
		work_in_action = true ;
		game->addGameEvent(AudioEffect::FinishResearch, game->getUnitByUID(this->unit_id).getView());
		return true;
	}
	if (action.code == "research_magiceconomy") {
	    magic_economy = true ;
		game->addGameEvent(AudioEffect::FinishResearch, game->getUnitByUID(this->unit_id).getView());
		return true;
	}
	return false;
}

std::string ComponentAcademy::getComponentInfo() const
{
	std::string str = "";
	if (work_in_action) str+="$Info_WorkInAction$\n" ;
	if (magic_economy) str+="$Info_MagicEconomy$\n" ;
	return str ;
}

bool ComponentAcademy::allowWorkWhileAction() const
{
	return work_in_action;
}

bool ComponentAcademy::isMagicEconomy() const
{
	return magic_economy;
}
