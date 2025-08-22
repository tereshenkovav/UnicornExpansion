#pragma once

#include <vector>
#include <string>
#include <map>
#include <set>
#include "GameUnit.h"
#include "BusyMap.h"
#include "UnitMover.h"
#include <SFML/Graphics.hpp>
#include <zetscript.h>
#include <json/json.h>
#include <optional>
#include "MushroomNet.h"
#include "Countdown.h"

// Перечисления территорий и типов лазера
enum class Terrain { Ground, Forest, Water, Road };
enum class LaserType { Harvest, Attack, Heal, Detox };
enum class AudioEffect { Teleport, FinishTeleport, FinishResearch, FinishUpgrade, UnderAttack };

// Запись для лазера
struct Laser {
	sf::Vector2f pos1;
	sf::Vector2f pos2;
	LaserType type;
	float timeshift;
};

// Основной класс игры - содержит всё, что обрабатывается в игровом цикле
class Game
{
private:
	int width;
	int height;
	std::optional<sf::Vector2i> new_viewpoint;
	float energy;
	std::vector<std::vector<Terrain>> map;
	std::vector<GameUnit> units;
	std::vector<Laser> lasers;
	Json::Value jsonActions;
	Json::Value jsonUnits;
	Json::Value jsonComponents;
	Json::Value jsonEnemys;
	Json::Value jsonDevConfig;
	std::string tasktext;
	MushroomNet mushrooms;
	float timerleft;
	bool iswin;
	bool isfail;
	std::vector<std::vector<bool>> fog;
	zetscript::ScriptEngine script_engine;
	std::function<bool()> funcvictory;
	std::function<bool()> funcdefeat;
	std::optional<sf::Vector2f> teleportation_effect;
	std::vector<AudioEffect> audioeffects;
	sf::Countdown counter_under_attack;
	std::set<int> last_attacked_units;
	std::set<int> new_attacked_units;
	std::optional<sf::Vector2f> lasteventpos;
	void trySetUnderAttackEffect(const GameUnit& unit);
public:
	Game();
	static bool canWalkOnTerrain(Terrain terr);
	// Загрузка данных из файлов
	bool loadMap(const std::string& filename);
	bool loadConfigs();
	bool loadDeveloperConfig(const std::string& filename);
	bool loadScript(const std::string& filename);
	// Работа с юнитами
	void addUnit(const GameUnit& unit);
	bool findUnitAt(float viewx, float viewy, int* uid) const;
	bool isUnitExist(int uid) const;
	// Размеры карты
	int getWidth() const;
	int getHeight() const;
	// Получить новую точку камеры и сброс новой точки
	std::optional<sf::Vector2i> getOnceNewViewPoint();
	void setNewViewPoint(int x, int y);
	// Получение типа территории
	Terrain getMap(int i, int j) const;
	bool isFog(int i, int j) const;
	void clearFogAt(const sf::Vector2i& pos, int dist);
	// Получение юнитов
	int getUnitCount() const;
	const GameUnit & getUnit(int i) const;
	const GameUnit & getUnitByUID(int uid) const;
	// Установка цели для движения юнита
	void setTargetToUnit(int uid, int targetx, int targety);
	// Отправить юниту действие
	bool sendUnitAction(int uid, const UnitAction & action);
	// Отмена действия у юнита
	bool cancelUnitWorkingAction(int uid);
	// Искать первую свободную точку рядом с юнитом
	std::optional<sf::Vector2i> getFirstFreePosNear(const GameUnit & unit) const;
	// Работа с энергией
	int getEnergy() const;
	bool decEnergy(int value);
	bool incEnergy(int value);
	// Получить информацию о единорогах текстовую
	std::string getUnicornCountInfo() const;
	// Получить информацию о таймере
	std::optional<std::string> getTimerStr() const;
	int getTimer() const;
	void startTimer(int value);
	int getUnicornCount() const;
	// Вычисление количества по компонентам
	int getCountByComponent(const std::string & compname) const;
	// Добавить компонент для юнита
	void addComponentToUnitByUID(int uid, UnitComponent* component);
	// Работа с лазерами
	int getLaserCount() const;
	Laser getLaser(int i) const;
	// Получение параметров игры
	Json::Value getConfigAction() const;
	Json::Value getConfigUnit() const;
	Json::Value getConfigComponent() const;
	Json::Value getConfigEnemy() const;
	// Функции проверки завершения игры
	bool isGameOver() const;
	bool isWin() const;
	bool isFail() const;
	// Текст игровой задачи
	std::string getTaskText() const;
	void setTaskText(const std::string & value);
	void addTeleportationEffect(float x, float y);
	void addGameEvent(AudioEffect effect, sf::Vector2f pos);
	std::optional<sf::Vector2f> getOnceTeleportationEffect();
	std::vector<AudioEffect> getOnceAudioEffects();
	const std::vector<Mushroom> & getMushrooms(int x, int y) const;
	bool isMushroomsAt(int x, int y) const;
	bool isMushroomsExist() const;
	bool isUnitUnderAttack(int uid) const;
	std::optional<sf::Vector2f> getLastEventPos() const;
	// Обновление игры
	void update(float dt);
	// Шаблонный метод - поиск юнитов по компонентам
	template <typename T> int getCountUnitWithComponent() const {
		int cnt = 0;
		for (size_t i = 0; i < units.size(); i++)
			if (units[i].isComponent<T>()) cnt++;
		return cnt;
	}
};

