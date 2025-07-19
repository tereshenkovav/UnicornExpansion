#pragma once

#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "BusyMap.h"
#include "UnitComponent.h"

// ������ ����� � ��������
const int BLOCKW = 64 ;
const int BLOCKH = 64 ;

// ���� ��������
enum class Moving { None, Left, Right, Up, Down } ;

// ������� ���� - ��� ������ �������� �������� ������������, � ����� ������ ��, ��� ����� ��� ����, �� ����������� ����� ��������
class GameUnit
{
private:
	float v ;
	float treach ;
	std::string caption;
	int maxhealth;
	std::string code;
	
	float worktek;
	float workall;
	UnitAction workaction;

	int uid;

	int targetx ;
	int targety ;
	bool targeted ;
	
	float tmoving ;
	Moving moving ;
	Moving lastmoving;
	int unitx ;
	int unity ;
	int unitw ;
	int unith ;
	float health;

	std::vector<UnitComponent*> components;
	int getMovingDx() const ;
	int getMovingDy() const ;
public:
	// �������� ����� �� ������� - �����, ������, ��������, ��� � �������� ����
	GameUnit(int unitx, int unity, int unitw, int unith, const std::string & caption, int maxhealth, const std::string & code);
	GameUnit();
	// � ������, ��� ���� ������ ���� ����������� - �� ��� ����� ����� ��������. �� ����� ����� ����� ������ ��������� �����.
	void setVelocity(int v);
	// ��������� ����� - ��� ����� �� �����, ��� ������ �� �����, ��� ����� � �������, ��� ������ � �������
	sf::Vector2f getView() const ;
	sf::Vector2f getSizeView() const;
	sf::Vector2i getXY() const;
	sf::Vector2i getSize() const;
	// ���������� ��� �����
	int getUID() const;
	// ��������� ���� �����
	sf::Vector2i getTarget() const ;
	// �������� ��������� ����� � �����
	bool isXYInUnit(float x, float y) const;
	// ���������� ����� �� ���������� ��������
	Moving getLastMoving() const;
	// ��������, ��������� �� ����� �����
	bool isUnitNearUnit(const GameUnit& unit) const;
	// �������� �����
	void moveTo(Moving moving) ;
	// ���������� ����� ��������� �� �����
	void updateBusyMap(BusyMap & busymap) const ;
	// ������ ���������� ����� �� �������
	void update(float dt) ;
	// �������� ����� - ��� ��������, ����� �� ����, �������� �� ��� ����������
	bool isMoving() const ;
	bool isTargeted() const ;
	bool isWorkingTask(float* progress, std::string * actioncode) const;
	bool isWorkingTask() const;
	// ��������� ���� � ����� �
	void setTarget(int x, int y) ;
	void resetTarget() ;
	// ��������� ������� ����� � ��� �������� ���������
	std::string getCaption() const;
	std::string getHealthInfo() const;
	float getHealthPerMax() const;
	int getVelocity() const;
	std::string getCode() const;
	std::vector<UnitAction> getActions() const;
	bool sendAction(const UnitAction & action);
	// ������ �� ���������
	void decHealth(float value);
	void incHealth(float value);
	// ������� ������� �����
	bool isKilled() const;
	// ����������� ������� ��������
	bool canSendAction(const UnitAction& action, std::string* msgcode) const;
	// ������ � ������������
	void addComponent(UnitComponent* comp);
	bool hasComponentByName(const std::string& code) const;
	std::string getComponentsInfo() const;
	template <typename T> bool isComponent() const {
		for (int i = 0; i < components.size(); i++) {
			auto* comp = dynamic_cast<T*>(components[i]);
			if (comp) return true;
		}
		return false;
	}
	template <typename T> T * getComponent() const {
		for (int i = 0; i < components.size(); i++) {
			auto* comp = dynamic_cast<T*>(components[i]);
			if (comp) return comp;
		}
		return NULL;
	}
	~GameUnit();
};
