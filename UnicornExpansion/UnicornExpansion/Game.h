#pragma once

#include <vector>
#include <string>
#include <map>
#include "GameUnit.h"
#include "BusyMap.h"
#include "UnitMover.h"
#include <SFML/Graphics.hpp>
#include <zetscript.h>
#include <json/json.h>

// ������������ ���������� � ����� ������
enum class Terrain { Ground, Forest, Water, Road };
enum class LaserType { Harvest, Attack, Heal };

// ������ ��� ������
struct Laser {
	sf::Vector2f pos1;
	sf::Vector2f pos2;
	LaserType type;
};

// �������� ����� ���� - �������� ��, ��� �������������� � ������� �����
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
	float timerleft;
	bool iswin;
	bool isfail;
	std::vector<std::vector<bool>> fog;
	zetscript::ScriptEngine script_engine;
	std::function<bool()> funcvictory;
	std::function<bool()> funcdefeat;
	static bool canWalkOnTerrain(Terrain terr);
public:
	Game();
	// �������� ������ �� ������
	bool loadMap(const std::string& filename);
	bool loadConfigs();
	bool loadDeveloperConfig(const std::string& filename);
	bool loadScript(const std::string& filename);
	// ������ � �������
	void addUnit(const GameUnit& unit);
	bool findUnitAt(float viewx, float viewy, int* uid) const;
	bool isUnitExist(int uid) const;
	// ������� �����
	int getWidth() const;
	int getHeight() const;
	// �������� ����� ����� ������ � ����� ����� �����
	std::optional<sf::Vector2i> getOnceNewViewPoint();
	void setNewViewPoint(int x, int y);
	// ��������� ���� ����������
	Terrain getMap(int i, int j) const;
	bool isFog(int i, int j) const;
	void clearFogAt(const sf::Vector2i& pos, int dist);
	// ��������� ������
	int getUnitCount() const;
	const GameUnit & getUnit(int i) const;
	const GameUnit & getUnitByUID(int uid) const;
	// ��������� ���� ��� �������� �����
	void setTargetToUnit(int uid, int targetx, int targety);
	// ��������� ����� ��������
	bool sendUnitAction(int uid, const UnitAction & action);
	// ������ ������ ��������� ����� ����� � ������
	std::optional<sf::Vector2i> getFirstFreePosNear(const GameUnit & unit) const;
	// ������ � ��������
	int getEnergy() const;
	bool decEnergy(int value);
	bool incEnergy(int value);
	// �������� ���������� � ���������� ���������
	std::string getUnicornCountInfo() const;
	// �������� ���������� � �������
	std::optional<std::string> getTimerStr() const;
	int getTimer() const;
	void startTimer(int value);
	int getUnicornCount() const;
	// ���������� ���������� �� �����������
	int getCountByComponent(const std::string & compname) const;
	// �������� ��������� ��� �����
	void addComponentToUnitByUID(int uid, UnitComponent* component);
	// ������ � ��������
	int getLaserCount() const;
	Laser getLaser(int i) const;
	// ��������� ���������� ����
	Json::Value getConfigAction() const;
	Json::Value getConfigUnit() const;
	Json::Value getConfigComponent() const;
	Json::Value getConfigEnemy() const;
	// ������� �������� ���������� ����
	bool isGameOver() const;
	bool isWin() const;
	bool isFail() const;
	// ����� ������� ������
	std::string getTaskText() const;
	void setTaskText(const std::string & value);
	// ���������� ����
	void update(float dt);
	// ��������� ����� - ����� ������ �� �����������
	template <typename T> int getCountUnitWithComponent() const {
		int cnt = 0;
		for (size_t i = 0; i < units.size(); i++)
			if (units[i].isComponent<T>()) cnt++;
		return cnt;
	}
};

