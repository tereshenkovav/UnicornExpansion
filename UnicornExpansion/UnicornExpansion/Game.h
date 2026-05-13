#pragma once

#include <vector>
#include <string>
#include <map>
#include <set>
#include "GameUnit.h"
#include "HelperCppClasses/Vector2D.h"
#include "UnitMover.h"
#include <SFML/Graphics.hpp>
#include <zetscript.h>
#include <json/json.h>
#include <optional>
#include "MushroomNet.h"
#include "HelperCppClasses/Countdown.h"
#include "SfmlGameEngine/Texts.h"

// Перечисления территорий и типов лазера
enum class Terrain { Ground, Forest, Water, Road };
enum class LaserType { Harvest, Attack, Heal, Detox };
enum class AudioEffect { Teleport, FinishTeleport, FinishResearch, FinishUpgrade, FinishBuilding, FinishConstruct, UnderAttack };

enum class Difficulty { Easy, Norm, Hard };

struct LevelCode {
	std::string company;
	int level;
	Difficulty difficulty;
};

// Запись для лазера
struct Laser {
	sf::Vector2f pos1;
	sf::Vector2f pos2;
	LaserType type;
	float timeshift;
};

struct Message {
	std::string icon;
	float duration;
	float showafter;
	std::string text;
};

enum class GameTaskStatus { Active, Completed, Cancelled };

struct GameTask {
	GameTaskStatus status;
	std::string code;
	std::string text;
};

// Основной класс игры - содержит всё, что обрабатывается в игровом цикле
class Game
{
private:
	int width;
	int height;
	std::optional<sf::Vector2i> new_viewpoint;
	float energy;
	sfge::Texts texts;
	Vector2D<Terrain> map;
	std::vector<GameUnit> units;
	std::vector<Laser> lasers;
	std::vector<Message> history;
	std::set<std::string> allowedactions;
	std::set<std::string> flags;
	Json::Value jsonActions;
	Json::Value jsonUnits;
	Json::Value jsonComponents;
	Json::Value jsonEnemys;
	Json::Value jsonDevConfig;
	std::vector<GameTask> tasks;
	MushroomNet mushrooms;
	float timerleft;
	bool istimervisible;
	bool iswin;
	bool isfail;
	Vector2D<bool> fog;
	Difficulty difficulty;
	zetscript::ScriptEngine script_engine;
	std::function<bool()> funcvictory;
	std::function<bool()> funcdefeat;
	std::function<void()> funcupdate;
	std::optional<sf::Vector2f> teleportation_effect;
	std::vector<AudioEffect> audioeffects;
	Countdown counter_under_attack;
	std::set<int> last_attacked_units;
	std::set<int> new_attacked_units;
	std::optional<sf::Vector2f> lasteventpos;
	bool magiceconomy = false;
	void trySetUnderAttackEffect(const GameUnit& unit);
	// Полезная функция, позволяет автоматически получать строки из внешнего файла, записывая их как $key, без использования game.getText
	std::string trText(const std::string& text) const;
	static GameUnit staticemptyunit;
public:
	Game();
	void setDifficulty(Difficulty value);
	static bool canWalkOnTerrain(Terrain terr);
	// Загрузка данных из файлов
	bool loadMap(const std::string& filename);
	bool loadConfigs();
	bool loadDeveloperConfig(const std::string& filename);
	bool loadScript(const std::string& filename);
	bool loadTexts(const std::string& filename);
	// Работа с юнитами
	void addUnit(const GameUnit& unit);
	std::optional<int> findUnitAt(float viewx, float viewy) const;
	std::vector<int> findVisibleUnitsInRect(float viewx1, float viewy1, float viewx2, float viewy2) const;
	// Размеры карты
	int getWidth() const;
	int getHeight() const;
	// Получить новую точку камеры и сброс новой точки
	std::optional<sf::Vector2i> getOnceNewViewPoint();
	// Получение типа территории
	Terrain getMap(int i, int j) const;
	bool isFog(int i, int j) const;
	void clearFogAt(const sf::Vector2i& pos, int dist);
	// Получение юнитов
	int getUnitCount() const;
	const GameUnit & getUnit(int i) const;
	const GameUnit & getUnitByUID(int uid) const;
	// Отправить юниту действие
	bool sendUnitAction(int uid, const UnitAction & action);
	// Отмена действия у юнита
	bool cancelUnitWorkingAction(int uid);
	// Искать первую свободную точку рядом с юнитом
	std::optional<sf::Vector2i> getFirstFreePosNear(const GameUnit & unit) const;

	/* Этот блок функций подключен в скрипты */
	// Работа с энергией
	int getEnergy() const;
	bool decEnergy(int value);
	bool incEnergy(int value);
	int getTimer() const;
	void startTimer(int value);
	void startHiddenTimer(int value);
	void addNewTask(const std::string& code, const std::string& text);
	void setTaskCompleted(const std::string& code);
	void setTaskCancelled(const std::string& code);
	bool isTaskCompleted(const std::string& code) const;
	// Вычисление количества по компонентам
	int getCountByComponent(const std::string& compname) const;
	bool isMushroomsExist() const;
	void setNewViewPoint(int x, int y);
	void addMessage(const std::string& icon, int duration, const std::string& text);
	void addMessageLater(const std::string& icon, int pause, int duration, const std::string& text);
	std::string getText(const std::string& name) const;
	void allowAction(const std::string& name);
	void denyAction(const std::string& name);
	void setFlag(const std::string& name);
	void clearFlag(const std::string& name);
	bool isFlag(const std::string& name) const;
	bool isUnitNearXY(int uid, int x, int y) const;
	bool isUnitNearXYDist(int uid, int x, int y, int dist) const;
	int getUnitHealth(int uid) const;
	bool isUnitExist(int uid) const;
	void deleteUnitLater(int uid);
	bool isHard1() const;
	bool isHard2() const;
	// Установка цели для движения юнита
	void setTargetToUnit(int uid, int targetx, int targety);
	/* Конец скриптового блока */

	// Получить информацию о таймере
	std::optional<std::string> getTimerStr() const;

	// Получить информацию о единорогах текстовую
	std::string getUnicornCountInfo() const;

	int getUnicornCount() const;
	// Добавить компонент для юнита
	void addComponentToUnitByUID(int uid, UnitComponent* component);
	// Установка щита для юнита
	void setShieldToUnit(int uid, int value);
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
	// Игровые задачи
	const std::vector<GameTask> & getTasks() const;
	void addTeleportationEffect(float x, float y);
	void addGameEvent(AudioEffect effect, sf::Vector2f pos);
	std::optional<sf::Vector2f> getOnceTeleportationEffect();
	std::vector<AudioEffect> getOnceAudioEffects();
	const std::vector<Mushroom> & getMushrooms(int x, int y) const;
	bool isMushroomsAt(int x, int y) const;
	bool isUnitUnderAttack(int uid) const;
	std::optional<sf::Vector2f> getLastEventPos() const;
	// Механизм сообщений
	void skipTekMessage();
	void skipAllMessages();
	std::vector<Message> getTekMessages() const;
	const std::vector<Message>& getHistory() const;
	bool isActionAllowed(const std::string name) const;
	bool isMagicEconomy() const;

	std::optional<sf::Vector2i> getFirstFreePosFor2x2Building(const GameUnit& unit) const;

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

