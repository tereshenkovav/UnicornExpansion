#include "Game.h"

#include <fstream>
#include <sstream>
#include <map>
#include <math.h>
#include <optional>
#include "HelperCppClasses/StringTools.h"
#include "ComponentUnicorn.h"
#include "ComponentHarvester.h"
#include "ComponentResource.h"
#include "ComponentAttacker.h"
#include "ComponentDetoxer.h"
#include "ComponentHealer.h"
#include "ComponentMeleeEnemy.h"
#include "ComponentEnemyTarget.h"
#include "ComponentEnemy.h"
#include "ComponentPortal.h"
#include "FinderByBestDistance.h"
#include "UnitFactory.h"
#include "SeedStore.h"
#include "ScriptWrapper.h"

std::string prepLine(const std::string& str) {
	return trimString(clearLineFromBOM(clearStringFromEndl(str)));
}

bool Game::canWalkOnTerrain(Terrain terr)
{
	// Здесь можно расширить фунции территорий
	return (terr==Terrain::Ground)||(terr==Terrain::Road);
}

void Game::trySetUnderAttackEffect(const GameUnit& unit)
{
	new_attacked_units.insert(unit.getUID());
	if (!counter_under_attack.isActive())
		if (last_attacked_units.count(unit.getUID()) == 0) {
			addGameEvent(AudioEffect::UnderAttack, unit.getView());
			counter_under_attack.upset(5.0f);
		}
}

Game::Game() {
	width = 0;
	height = 0;
	iswin = false;
	isfail = false;

	current_game = this;
	// Резервируем место для 1024 юнитов, чтобы они не перемещались в памяти
        // при push_back, поскольку это портит данные при добавлении
        // нового юнита внутри кода юнита (обработка действий)
	units.reserve(1024);
}

bool Game::loadMap(const std::string& filename) {
	std::map<char, Terrain> mapchars;
	mapchars['+'] = Terrain::Ground;
	mapchars['F'] = Terrain::Forest;
	mapchars['W'] = Terrain::Water;
	mapchars['R'] = Terrain::Road;

	std::string line;
	std::ifstream fin(filename);
	if (!fin.is_open()) return false;

	if (!std::getline(fin, line)) return false;
	line = prepLine(line);
	if (!isStringNumber(line)) return false;
	width = std::stoi(line);

	if (!std::getline(fin, line)) return false;
	line = prepLine(line);
	if (!isStringNumber(line)) return false;
	height = std::stoi(line);
		
	// Необходимо, чтобы сеть грибов была инициализирована до выполнения скриптов и до установки территории
	mushrooms.initByGame(this);

	map.setOutboundValue(Terrain::Ground);
	map.resizeAndFill(width, height, Terrain::Ground);
	
	for (int j = 0; j < height; j++) {
		if (!std::getline(fin, line)) return false;
		line = prepLine(line);

		for (int i = 0; i < width; i++)
			if (i < line.length()) {
				char c = line[i];
				if ((c >= '1') && (c <= '0' + MAX_MUSHROOMS)) {
					mushrooms.setMushrooms(i, j, c - '0');
					c = '+';
				}
				if (mapchars.count(c) > 0)
					map.setValue(i, j, mapchars[c]);
			}
	}
	
	fin.close();

	fog.setOutboundValue(true);
	fog.resizeAndFill(width, height, !jsonDevConfig["clearfog"].asBool());
	return true;
}

bool Game::loadConfigs() {
	Json::CharReaderBuilder readerBuilder;
	std::ifstream jsonFile1("configs/enemys.json", std::ifstream::binary);
	std::ifstream jsonFile2("configs/actions.json", std::ifstream::binary);
	std::ifstream jsonFile3("configs/units.json", std::ifstream::binary);
	std::ifstream jsonFile4("configs/components.json", std::ifstream::binary);
	std::string errors;

	if (!Json::parseFromStream(readerBuilder, jsonFile1, &jsonEnemys, &errors)) return false;
	if (!Json::parseFromStream(readerBuilder, jsonFile2, &jsonActions, &errors)) return false;
	if (!Json::parseFromStream(readerBuilder, jsonFile3, &jsonUnits, &errors)) return false;
	if (!Json::parseFromStream(readerBuilder, jsonFile4, &jsonComponents, &errors)) return false;

	return true;
}

bool Game::loadDeveloperConfig(const std::string& filename)
{
	Json::CharReaderBuilder readerBuilder;
	std::ifstream jsonFile(filename, std::ifstream::binary);
	std::string errors;
	if (!Json::parseFromStream(readerBuilder, jsonFile, &jsonDevConfig, &errors)) return false;
	return true;
}

bool Game::loadScript(const std::string& filename) {
	units.clear();
	allowedactions.clear();
	energy = 0.0f;
	tasktext = "?";
	timerleft = 0;

	try {
		script_engine.clear();
		registerTypeGameInScript(script_engine);
		script_engine.compileFile(zetscript::String(filename.c_str()));
		auto initMap = script_engine.bindScriptFunction<bool()>("initMap");
		funcvictory = script_engine.bindScriptFunction<bool()>("isVictory");
		funcdefeat = script_engine.bindScriptFunction<bool()>("isDefeat");
		initMap();
	}
	catch (std::exception& e) {
		// Экспресс-реализация записи ошибки
		std::ofstream fout("errog.log");
		if (fout.is_open()) {
			fout << e.what() << std::endl;
			fout.close();
		}
	}
		
	// Пришлось добавить сюда, чтобы где-то инициализировало
	iswin = false;
	isfail = false;
	counter_under_attack.reset();
	last_attacked_units.clear();
	lasteventpos = std::nullopt;
	// Очистка лазеров
	lasers.clear();
	
	return true;
}

bool Game::loadTexts(const std::string& filename)
{
	texts.loadFromFile(filename);
	return true;
}

bool Game::isUnitExist(int uid) const
{
	for (int i = 0; i < units.size(); i++)
		if (units[i].getUID() == uid) return true;
	return false;
}

int Game::getWidth() const {
	return width;
}

int Game::getHeight() const {
	return height;
}

Terrain Game::getMap(int i, int j) const {
	return map.getValue(i, j);
}

void Game::addUnit(const GameUnit & unit) {
	units.push_back(unit);
}

std::optional<int> Game::findUnitAt(float viewx, float viewy) const
{
	// Примитивная реализация
	for (int i = 0; i < units.size(); i++)
		if (units[i].isXYInUnit(viewx, viewy))
			return units[i].getUID();
	return std::nullopt;
}

std::vector<int> Game::findVisibleUnitsInRect(float viewx1, float viewy1, float viewx2, float viewy2) const {
	std::vector<int> uids;
	for (int i = 0; i < units.size(); i++)
		if (units[i].isUnitInRect(viewx1, viewy1, viewx2, viewy2))
			if (!isFog(units[i].getXY().x, units[i].getXY().y))
				uids.push_back(units[i].getUID());
	return uids;
}

int Game::getUnitCount() const {
	return units.size();
}

const GameUnit& Game::getUnit(int i) const
{
	return units[i];
}

const GameUnit& Game::getUnitByUID(int uid) const
{
	for (int i = 0; i < units.size(); i++)
		if (units[i].getUID() == uid)
			return units[i];
	return GameUnit();
}

void Game::setTargetToUnit(int uid, int targetx, int targety)
{
	for (int i = 0; i < units.size(); i++)
		if (units[i].getUID() == uid) {
			units[i].setTarget(targetx, targety);
			return;
		}
}

bool Game::sendUnitAction(int uid, const UnitAction & action)
{
	for (int i = 0; i < units.size(); i++)
		if (units[i].getUID() == uid) return units[i].sendAction(action);
	return false;
}

bool Game::cancelUnitWorkingAction(int uid)
{
	for (int i = 0; i < units.size(); i++)
		if (units[i].getUID() == uid) return units[i].cancelWorkingAction();
	return false;
}

std::optional<sf::Vector2i> Game::getFirstFreePosNear(const GameUnit & unit) const
{
	std::vector<sf::Vector2i> nears;
	for (int i = -1; i < unit.getSize().x + 1; i++)
		nears.push_back({ i, -1 });

	for (int i = 0; i < unit.getSize().y + 1; i++)
		nears.push_back({ unit.getSize().x, i });

	for (int i = unit.getSize().x - 1; i >= -1; i--)
		nears.push_back({ i, unit.getSize().y });

	for (int i = unit.getSize().y - 1; i >= 0; i--)
		nears.push_back({ -1, i });

	Vector2D<bool> busymap(width, height,false);
	busymap.setOutboundValue(false) ;

	// Нужно вынести в отдельный блок построения полной карты занятости
	for (int x = 0; x < width; x++)
		for (int y = 0; y < height; y++)
			if (!canWalkOnTerrain(getMap(x, y))) busymap.setValue(x, y, true);
	for (int j = 0; j < units.size(); j++)
		units[j].updateBusyMap(busymap);

	for (auto i = 0; i < nears.size(); i++)
		if (!busymap.getValue(unit.getXY().x + nears[i].x, unit.getXY().y + nears[i].y))
			return sf::Vector2i({unit.getXY().x + nears[i].x, unit.getXY().y + nears[i].y });

	return std::nullopt;
}

int Game::getEnergy() const
{
	return (int)energy;
}

bool Game::decEnergy(int value)
{
	if (getEnergy() < value) return false;
	energy -= value;
	return true;
}

bool Game::incEnergy(int value)
{
	energy += value;
	return true;
}

std::string Game::getUnicornCountInfo() const
{
	int max = 0;
	for (int i = 0; i < units.size(); i++)
		if (units[i].isComponent<ComponentPortal>()) max += units[i].getComponent<ComponentPortal>()->getMaxUnicornCount();
	return std::format("{}/{}", getUnicornCount(), max);
}

int Game::getUnicornCount() const
{
	return std::count_if(units.begin(), units.end(), [](auto& unit) {return unit.template isComponent<ComponentUnicorn>(); });
}

int Game::getCountByComponent(const std::string& compname) const
{
	return std::count_if(units.begin(), units.end(), [compname](auto& unit) {return unit.hasComponentByName("Component" + compname); });
}

std::string Game::getTaskText() const
{
	return tasktext;
}

void Game::setTaskText(const std::string & task)
{
	tasktext = task;
}

void Game::addTeleportationEffect(float x, float y)
{
	teleportation_effect = { x,y };
}

void Game::addGameEvent(AudioEffect effect, sf::Vector2f pos)
{
	audioeffects.push_back(effect);
	lasteventpos = pos;
}

std::optional<sf::Vector2f> Game::getOnceTeleportationEffect()
{
	if (teleportation_effect) {
		auto buf = teleportation_effect;
		teleportation_effect = std::nullopt;
		return buf;
	}
	return std::nullopt;
}

std::vector<AudioEffect> Game::getOnceAudioEffects()
{
	auto buf = audioeffects;
	audioeffects.clear();
	return buf;
}

const std::vector<Mushroom>& Game::getMushrooms(int x, int y) const
{
	return mushrooms.getMushrooms(x, y);
}

bool Game::isMushroomsAt(int x, int y) const
{
	return mushrooms.getMushroomStage(x, y) > 0;
}

bool Game::isMushroomsExist() const
{
	return mushrooms.isMushroomsExist();
}

bool Game::isUnitUnderAttack(int uid) const
{
	return last_attacked_units.count(uid) > 0;
}

std::optional<sf::Vector2f> Game::getLastEventPos() const
{
	return lasteventpos;
}

void Game::addComponentToUnitByUID(int uid, UnitComponent* component)
{
	for (int i = 0; i < units.size(); i++)
		if (units[i].getUID() == uid) units[i].addComponent(component);
}

void Game::setShieldToUnit(int uid, int value) {
	for (int i = 0; i < units.size(); i++)
		if (units[i].getUID() == uid) units[i].setShield(value);
}

int Game::getLaserCount() const
{
	return lasers.size();
}

Laser Game::getLaser(int i) const
{
	return lasers[i];
}

Json::Value Game::getConfigEnemy() const {
	return jsonEnemys;
}

Json::Value Game::getConfigUnit() const {
	return jsonUnits;
}

Json::Value Game::getConfigAction() const {
	return jsonActions;
}

Json::Value Game::getConfigComponent() const {
	return jsonComponents;
}

void Game::update(float dt)
{
	// Сначала обновляем карту занятости ячеек
	Vector2D<bool> busymap(width, height,false);
	
	new_attacked_units.clear();

	if (timerleft > 0.0f) timerleft -= dt;

	for (int x = 0; x < width; x++)
		for (int y = 0; y < height; y++)
			if (!canWalkOnTerrain(getMap(x, y))) busymap.setValue(x, y, true);

	// Потом для каждого юнита обновляем всю карту занятости по новой, и двигаем его. 
	// Иначе два юнита могут войти в одну свободную ячейку
	for (int i = 0; i < units.size(); i++) {
		auto localbusymap = busymap;
		UnitMover mover(&localbusymap);
		for (int j = 0; j < units.size(); j++)
			units[j].updateBusyMap(localbusymap);

		mover.updateUnit(units[i]);
	}

	// И потом уже работаем с обновлением юнитов
	for (int i = 0; i < units.size(); i++) {
		units[i].update(dt);
		// Для единоров Разгон тумана войны
		if (units[i].isComponent<ComponentUnicorn>()) clearFogAt(units[i].getXY(), 6);
	}
	
	// Обновление урона от грибов для единорогов
	for (int i = 0; i < units.size(); i++)
		if (units[i].isComponent<ComponentUnicorn>())
			if (int stage = mushrooms.getMushroomStage(units[i].getXY().x, units[i].getXY().y); stage > 0) {
				units[i].decHealth(stage * dt);
				trySetUnderAttackEffect(units[i]);
			}

	mushrooms.update(dt);
	counter_under_attack.update(dt);
	counter_showmessage.update(dt);

	if ((!counter_showmessage.isActive())&&(!messages.empty())) {
		history.push_back(messages.front());
		counter_showmessage.upset(messages.front().duration);
		messages.pop();
	}


	// Временная поправка для позиции лазера у единорога
	sf::Vector2f laserfixleft{ -23, -25 };
	sf::Vector2f laserfixright{ 21, -25 };

	// Построение лазеров для рендера и действия с ними
	lasers.clear();
	for (int i = 0; i < units.size(); i++) {
		if (const auto* harvester = units[i].getComponent<ComponentHarvester>()) {
			if ((!units[i].isWorkingTask())&&(!units[i].isTargeted())) {
				FinderByBestDistance finder(harvester->getHarvestDistance(), units[i].getView());
				for (int j = 0; j < units.size(); j++)
					if (units[j].isComponent<ComponentResource>())
						finder.addPos(units[j].getView(), j);
				if (auto res_idx = finder.getBestIndex()) {
					energy += harvester->getHarvestRate() * dt;
					units[*res_idx].decHealth(harvester->getHarvestRate() * dt);
					lasers.push_back({ units[i].getView() + (units[i].getLastMoving() == Moving::Left ? laserfixleft : laserfixright),
						units[*res_idx].getView(), LaserType::Harvest, SeedStore::getSeedByUIDAndLaserType(units[i].getUID(),LaserType::Harvest) });
				}
			}
		}
		if (auto* healer = units[i].getComponent<ComponentHealer>()) {
			if ((!units[i].isWorkingTask()) && (!units[i].isTargeted())) {
				FinderByBestDistance finder(healer->getHealerDistance(), units[i].getView());
				for (int j = 0; j < units.size(); j++)
					if (units[j].isComponent<ComponentUnicorn>())
						if (units[j].getHealthPerMax()<1.0) // Если юнит поврежден
							if (units[j].getUID() != units[i].getUID()) // Себя лечить нельзя
								finder.addPos(units[j].getView(), j);
				if (auto res_idx = finder.getBestIndex()) {
					if (energy >= healer->getHealerEnergyCost() * dt) {
						if (!healer->isLocked()) {
							energy -= healer->getHealerEnergyCost() * dt;
							healer->setActive(true);
							units[*res_idx].incHealth(healer->getHealerRate() * dt);
							lasers.push_back({ units[i].getView() + (units[i].getLastMoving() == Moving::Left ? laserfixleft : laserfixright),
								units[*res_idx].getView(), LaserType::Heal, SeedStore::getSeedByUIDAndLaserType(units[i].getUID(),LaserType::Heal) });
						}
					}
					else {
						if (healer->isActive()) healer->LockTemporary();
					}
				}
			}
		}
		if (const auto* attacker = units[i].getComponent<ComponentAttacker>()) {
			if ((!units[i].isWorkingTask()) && (!units[i].isTargeted())) {
				FinderByBestDistance finder(attacker->getAttackDistance(), units[i].getView());
				// Сначала ищем врагов, которые могут атаковать танк в ответ
				for (int j = 0; j < units.size(); j++)
					if (units[j].isComponent<ComponentMeleeEnemy>())
						finder.addPos(units[j].getView(), j);
				auto res_idx = finder.getBestIndex();
				// И если не нашли, тогда ищем вообще всех врагов
				if (!res_idx) {
					finder.reset();
					for (int j = 0; j < units.size(); j++)
						if (units[j].isComponent<ComponentEnemy>())
							finder.addPos(units[j].getView(), j);
				}
				// После этого, уже пытаемся атаковать
				if (auto res_idx = finder.getBestIndex()) {
					units[*res_idx].decHealth(attacker->getAttackValue() * dt);
					lasers.push_back({ units[i].getView() + (units[i].getLastMoving() == Moving::Left ? laserfixleft : laserfixright),
						units[*res_idx].getView(), LaserType::Attack, SeedStore::getSeedByUIDAndLaserType(units[i].getUID(),LaserType::Attack) });
					if (auto* meleeenemy = units[*res_idx].getComponent<ComponentMeleeEnemy>())
						meleeenemy->setTargetToUnit(units[i].getUID());
				}
			}
		}
		if (const auto* detoxer = units[i].getComponent<ComponentDetoxer>()) {
			if ((!units[i].isWorkingTask()) && (!units[i].isTargeted())) {
				FinderByBestDistance finder(detoxer->getDetoxDistance(), units[i].getView());
				// Ищем грибные зоны
				for (int x = 0; x < width; x++)
					for (int y = 0; y < height; y++)
						for (auto m : mushrooms.getMushrooms(x, y))
							finder.addPos(sf::Vector2f(BLOCKW * x + m.x, BLOCKH * y + m.y), m.index);
				if (auto res_idx = finder.getBestIndex()) {
					mushrooms.attackMushrooms(*res_idx, detoxer->getDetoxValue()* dt);
					lasers.push_back({ units[i].getView() + (units[i].getLastMoving() == Moving::Left ? laserfixleft : laserfixright),
						*finder.getBestPos(), LaserType::Detox, SeedStore::getSeedByUIDAndLaserType(units[i].getUID(),LaserType::Detox) });
				}
			}
		}
		if (auto* enemy = units[i].getComponent<ComponentMeleeEnemy>()) {
			// Блок отвечает за атаку на танк в зоне атаки - ближнее соприкосновение с учетом размеров объектов
			std::vector<int> targets;
			for (int j = 0; j < units.size(); j++)
				if (units[j].isComponent<ComponentEnemyTarget>() && units[i].isUnitNearUnit(units[j]))
					targets.push_back(j);

			if (targets.size() > 0) {
				std::optional<int> idx_attack = std::nullopt;
				// Сначала пробуем атаковать именно те юниты, которые  могут атаковать в ответ
				for (int j : targets)
					if (units[j].isComponent<ComponentAttacker>()) idx_attack = j;
				// И если танки не нашли, то все остальные цели, любая
				if (!idx_attack) idx_attack = targets[0];

				units[*idx_attack].decHealth(enemy->getAttackValue() * dt);
				trySetUnderAttackEffect(units[*idx_attack]);
			}

			// Блок отвечает за преследование танка, вошедшего в зону зрения
			FinderByBestDistance finder_view(enemy->getViewDistance(), units[i].getView());
			// Аналогично, сначала ищем вражеские юниты, способные атаковать
			for (int j = 0; j < units.size(); j++)
				if (units[j].isComponent<ComponentEnemyTarget>() && units[j].isComponent<ComponentAttacker>())
					finder_view.addPos(units[j].getView(), j);
			auto res_idx = finder_view.getBestIndex();
			if (!res_idx) {
				// Если не нашли, тогда все остальные юниты, которые можно атаковать
				finder_view.reset();
				for (int j = 0; j < units.size(); j++)
					if (units[j].isComponent<ComponentEnemyTarget>())
						finder_view.addPos(units[j].getView(), j);
			}
			if (auto res_idx = finder_view.getBestIndex())
				enemy->setTargetToUnit(units[*res_idx].getUID());
		}
	}

	last_attacked_units = new_attacked_units;

	// При удалении единорога дать эффект вспышки (обработка перед самым удалением)
	for (auto & unit: units)
		if (unit.isKilled() && unit.isComponent<ComponentUnicorn>()) {
			addTeleportationEffect(unit.getView().x, unit.getView().y);
			addGameEvent(AudioEffect::Teleport, unit.getView());
		}

	// Удаление уничтоженных юнитов
	std::erase_if(units, [](const GameUnit& unit){ return unit.isKilled(); });

	try {
		if (!isGameOver()) iswin = funcvictory();
		// Обязательно в независимом блоке, чтобы сначала проверяли победу, и если не найдено, то  потом поражение
		if (!isGameOver()) isfail = funcdefeat();
	}
	catch (std::exception& e) {
		// Экспресс-реализация записи ошибки
		std::ofstream fout("errortest.log");
		if (fout.is_open()) {
			fout << e.what() << std::endl;
			fout.close();
		}
	}
}

bool Game::isFog(int i, int j) const
{
	return fog.getValue(i, j);
}

void Game::clearFogAt(const sf::Vector2i& pos, int dist)
{
	for (int dx = -dist; dx <= dist; dx++)
		for (int dy = -dist; dy <= dist; dy++)
			if (dx * dx + dy * dy < dist * dist) fog.setValue(pos.x + dx, pos.y + dy, false);
}

bool Game::isGameOver() const
{
	return isfail || iswin;
}

std::optional<std::string> Game::getTimerStr() const
{
	return (timerleft > 0)? std::optional(std::format("{:02}:{:02}", (int)(timerleft / 60), (int)(timerleft) % 60)): std::nullopt;
}

int Game::getTimer() const
{
	return (int)(timerleft);
}

void Game::startTimer(int value)
{
	timerleft = value;
}

bool Game::isWin() const
{
	return iswin;
}

bool Game::isFail() const
{
	return isfail;
}

std::optional<sf::Vector2i> Game::getOnceNewViewPoint() {
	if (new_viewpoint) {
		sf::Vector2i buf = *new_viewpoint;
		new_viewpoint = std::nullopt;
		return buf;
	}
	return std::nullopt;
}

void Game::setNewViewPoint(int x, int y) {
	new_viewpoint = { x, y };
}

void Game::addMessage(const std::string& icon, int duration, const std::string& text) {
	// Полезная функция, позволяет автоматически получать строки из внешнего файла, записывая их как $key, без использования game.getText
	messages.push({ icon, duration, (text.at(0) == '$')?getText(text.substr(1)):text});
}

void Game::skipTekMessage() {
	counter_showmessage.reset();
}

void Game::skipAllMessages() {
	skipTekMessage();
	while (!messages.empty()) messages.pop();
}

std::optional<Message> Game::getTekMessage() const {
	if (history.empty()) return std::nullopt;
	if (!counter_showmessage.isActive()) return std::nullopt;
	return history.back();
}

bool Game::isActionAllowed(const std::string name) const
{
	return allowedactions.contains(name);
}

std::string Game::getText(const std::string& name) const
{
	return texts.getStr(name);
}

void Game::allowAction(const std::string& name)
{
	if (!allowedactions.contains(name)) allowedactions.insert(name);
}

void Game::denyAction(const std::string& name)
{
	if (allowedactions.contains(name)) allowedactions.erase(name);
}
