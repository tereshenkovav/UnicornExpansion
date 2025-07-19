#include "Game.h"

#include <fstream>
#include <sstream>
#include <map>
#include <math.h>
#include "CppTools.h"
#include "ComponentUnicorn.h"
#include "ComponentHarvester.h"
#include "ComponentResource.h"
#include "ComponentAttacker.h"
#include "ComponentHealer.h"
#include "ComponentMeleeEnemy.h"
#include "ComponentEnemyTarget.h"
#include "ComponentEnemy.h"
#include "ComponentPortal.h"
#include "FinderByBestDistance.h"
#include "UnitFactory.h"
#include "ScriptWrapper.h"

std::string prepLine(const std::string& str) {
	return trimString(clearLineFromBOM(clearStringFromEndl(str)));
}

bool Game::canWalkOnTerrain(Terrain terr)
{
	// Здесь можно расширить фунции территорий
	return (terr==Terrain::Ground)||(terr==Terrain::Road);
}

Game::Game() {
	width = 0;
	height = 0;
	iswin = false;
	isfail = false;

	current_game = this;
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

	if (!std::getline(fin, line)) return false;
	line = prepLine(line);
	if (!isStringNumber(line)) return false;
	initial_x = std::stoi(line);

	if (!std::getline(fin, line)) return false;
	line = prepLine(line);
	if (!isStringNumber(line)) return false;
	initial_y = std::stoi(line);

	map.resize(width);
	for (int j = 0; j < width; j++)
		map[j].resize(height);

	for (int j = 0; j < height; j++) {
		if (!std::getline(fin, line)) return false;
		line = prepLine(line);

		for (int i = 0; i < width; i++)
			if (i < line.length())
				if (mapchars.count(line[i]) > 0)
					map[i][j] = mapchars[line[i]];
	}
	
	fin.close();

	bool clearfog = jsonDevConfig["clearfog"].asBool();
	fog.resize(width);
	for (int j = 0; j < width; j++) {
		fog[j].resize(height);
		for (int i = 0; i < height; i++)
			fog[j][i] = !clearfog;
	}

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
	// Очистка лазеров
	lasers.clear();

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
	if (i < 0) return Terrain::Ground;
	if (i >= width) return Terrain::Ground;
	if (j < 0) return Terrain::Ground;
	if (j >= height) return Terrain::Ground;
	return map[i][j];
}

void Game::addUnit(const GameUnit & unit) {
	units.push_back(unit);
}

bool Game::findUnitAt(float viewx, float viewy, int* uid) const
{
	// Примитивная реализация
	for (int i=0; i<units.size(); i++)
		if (units[i].isXYInUnit(viewx,viewy)) {
			*uid = units[i].getUID();
			return true;
		}
	return false;
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

	BusyMap busymap(width, height);

	// Нужно вынести в отдельный блок построения полной карты занятости
	for (int x = 0; x < width; x++)
		for (int y = 0; y < height; y++)
			if (!canWalkOnTerrain(getMap(x, y))) busymap.setBusy(x, y);
	for (int j = 0; j < units.size(); j++)
		units[j].updateBusyMap(busymap);

	for (auto i = 0; i < nears.size(); i++)
		if (!busymap.isBusy(unit.getXY().x + nears[i].x, unit.getXY().y + nears[i].y))
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
	return std::to_string(getUnicornCount())+"/"+std::to_string(max);
}

int Game::getUnicornCount() const
{
	int r = 0;
	for (int i = 0; i < units.size(); i++)
		if (units[i].isComponent<ComponentUnicorn>()) r++;
	return r;
}

int Game::getCountByComponent(const std::string& compname) const
{
	int r = 0;
	for (int i = 0; i < units.size(); i++)
		if (units[i].hasComponentByName("Component"+compname)) r++;
	return r;
}

std::string Game::getTaskText() const
{
	return tasktext;
}

void Game::setTaskText(const std::string & task)
{
	tasktext = task;
}

void Game::addComponentToUnitByUID(int uid, UnitComponent* component)
{
	for (int i = 0; i < units.size(); i++)
		if (units[i].getUID() == uid) units[i].addComponent(component);
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
	BusyMap busymap(width, height);

	if (timerleft > 0.0f) timerleft -= dt;

	for (int x = 0; x < width; x++)
		for (int y = 0; y < height; y++)
			if (!canWalkOnTerrain(getMap(x, y))) busymap.setBusy(x, y);

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
		// Разгон тумана войны
		if (units[i].isComponent<ComponentUnicorn>()) clearFogAt(units[i].getXY(), 6);
	}

	// Временная поправка для позиции лазера
	sf::Vector2f laserfixleft{ -23, -25 };
	sf::Vector2f laserfixright{ 23, -25 };

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
						units[*res_idx].getView(), LaserType::Harvest });
				}
			}
		}
		if (const auto* healer = units[i].getComponent<ComponentHealer>()) {
			if ((!units[i].isWorkingTask()) && (!units[i].isTargeted())) {
				FinderByBestDistance finder(healer->getHealerDistance(), units[i].getView());
				for (int j = 0; j < units.size(); j++)
					if (units[j].isComponent<ComponentUnicorn>())
						if (units[j].getHealthPerMax()<1.0) // Если юнит поврежден
							if (units[j].getUID() != units[i].getUID()) // Себя лечить нельзя
								finder.addPos(units[j].getView(), j);
				if (auto res_idx = finder.getBestIndex()) {
					if (energy >= healer->getHealerEnergyCost() * dt) {
						energy -= healer->getHealerEnergyCost() * dt;
						units[*res_idx].incHealth(healer->getHealerRate() * dt);
						lasers.push_back({ units[i].getView() + (units[i].getLastMoving() == Moving::Left ? laserfixleft : laserfixright),
							units[*res_idx].getView(), LaserType::Heal });
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
						units[*res_idx].getView(), LaserType::Attack });
					if (auto* meleeenemy = units[*res_idx].getComponent<ComponentMeleeEnemy>())
						meleeenemy->setTargetToUnit(units[i].getUID());
				}
			}
		}
		if (auto* enemy = units[i].getComponent<ComponentMeleeEnemy>()) {
			// Блок отвечает за атаку на танк в зоне атаки - ближнее соприкосновение с учетом размеров объектов
			// Сначала пробуем атаковать именно те юниты, которые  могут атаковать в ответ
			bool findtank = false;
			for (int j = 0; j < units.size(); j++)
				if (units[j].isComponent<ComponentEnemyTarget>()&& units[j].isComponent<ComponentAttacker>())
					if (units[i].isUnitNearUnit(units[j])) {
						units[j].decHealth(enemy->getAttackValue() * dt);
						findtank = true;
						break;
					}
			// И если танки не нашли, то все остальные цели
			if (!findtank)
				for (int j = 0; j < units.size(); j++)
					if (units[j].isComponent<ComponentEnemyTarget>())
						if (units[i].isUnitNearUnit(units[j])) {
							units[j].decHealth(enemy->getAttackValue() * dt);
							break;
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

	// Удаление уничтоженных юнитов
	int i = 0;
	while (i < units.size())
		if (units[i].isKilled()) units.erase(units.begin() + i); else i++;

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
	if (i < 0) return true;
	if (i >= width) return true;
	if (j < 0) return true;
	if (j >= height) return true;
	return fog[i][j];
}

void Game::clearFogAt(const sf::Vector2i& pos, int dist)
{
	for (int dx = -dist; dx <= dist; dx++)
		for (int dy = -dist; dy <= dist; dy++) {
			if (dx * dx + dy * dy >= dist * dist) continue;
			int nx = pos.x + dx;
			int ny = pos.y + dy;
			if (nx < 0) continue;
			if (nx >= width) continue;
			if (ny < 0) continue;
			if (ny >= height) continue;
			fog[nx][ny] = false;
		}
}

bool Game::isGameOver() const
{
	return isfail || iswin;
}

std::optional<std::string> Game::getTimerStr() const
{
	if (timerleft > 0) {
		std::stringstream ss;
		ss << std::setw(2) << std::setfill('0') << (int)(timerleft / 60);
		ss << ":";
		ss << std::setw(2) << std::setfill('0') << (int)(timerleft) % 60;
		return ss.str();
	}
	else
		return std::nullopt;
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

sf::Vector2i Game::getInitialView() const {
	return { initial_x, initial_y };
}
