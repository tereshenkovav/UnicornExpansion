#pragma once

#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "BusyMap.h"
#include "UnitComponent.h"

// Размер юнита в пикселях
const int BLOCKW = 64 ;
const int BLOCKH = 64 ;

// Типы движения
enum class Moving { None, Left, Right, Up, Down } ;

// Базовый юнит - его особые свойства задаются компонентами, а здесь только то, что общее для всех, за исключением опций движения
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
	// Создание юнита по шаблону - место, размер, название, код и здоровье макс
	GameUnit(int unitx, int unity, int unitw, int unith, const std::string & caption, int maxhealth, const std::string & code);
	GameUnit();
	// В идеале, это тоже должно быть компонентом - не все юниты имеют скорость. Но тогда будет очень сильно связанный класс.
	void setVelocity(int v);
	// Параметры юнита - его место на карте, его размер на карте, его место в ячейках, его размер в ячейках
	sf::Vector2f getView() const ;
	sf::Vector2f getSizeView() const;
	sf::Vector2i getXY() const;
	sf::Vector2i getSize() const;
	// Уникальный код юнита
	int getUID() const;
	// Получение цели юнита
	sf::Vector2i getTarget() const ;
	// Проверка вхождение точки в юнита
	bool isXYInUnit(float x, float y) const;
	// Ориентация юнита по последнему движению
	Moving getLastMoving() const;
	// Проверка, находятся ли юниты рядом
	bool isUnitNearUnit(const GameUnit& unit) const;
	// Движение юнита
	void moveTo(Moving moving) ;
	// Обновление карты занятости от юнита
	void updateBusyMap(BusyMap & busymap) const ;
	// Просто обновление юнита по времени
	void update(float dt) ;
	// Свойства юнита - его движение, имеет ли цель, работает ли над улучшением
	bool isMoving() const ;
	bool isTargeted() const ;
	bool isWorkingTask(float* progress, std::string * actioncode) const;
	bool isWorkingTask() const;
	// Установка цели и сброс её
	void setTarget(int x, int y) ;
	void resetTarget() ;
	// Получение свойств юнита и его действия доступные
	std::string getCaption() const;
	std::string getHealthInfo() const;
	float getHealthPerMax() const;
	int getVelocity() const;
	std::string getCode() const;
	std::vector<UnitAction> getActions() const;
	bool sendAction(const UnitAction & action);
	// Работа со здоровьем
	void decHealth(float value);
	void incHealth(float value);
	// Признак убитого юнита
	bool isKilled() const;
	// Возможность вызвать действие
	bool canSendAction(const UnitAction& action, std::string* msgcode) const;
	// Работа с компонентами
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
