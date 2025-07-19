#pragma once

#include <vector>
#include <string>

class UnitComponent;
class Game;

// Запись для действия - его код, надпись, энергия и время, а также ссылка на компонент, где действие находится
struct UnitAction {
	std::string code;
	std::string caption;
	int energy;
	int time;
	UnitComponent* component;
};

// Класс компонента - от него создаем прочие.
// Компоненты задают свойства юнитов игры без иерархии наследования самих юнитов, позволяя сложные конструкции юнитов.
class UnitComponent
{
protected:
	Game * game;
	int unit_id;
public:
	// Создание компонента с игрой
	UnitComponent(Game * game);
	// Установить юнит, в котором компонент находится
	void setUnitID(int unit_id);
	virtual ~UnitComponent();
	// Виртуальные методы для перегрузки в потомках, по умолчанию они делают базовые вещи
	// Действия
	virtual std::vector<UnitAction> getActions() const;
	// Применение действий - в момент завершения и в момент старта
	virtual bool applyAction(const UnitAction& action);
	virtual bool canApplyAction(const UnitAction& action, std::string* msgcode) const;
	virtual bool doPrefixAction(const UnitAction& action);
	// Получение свойств компонента
	virtual std::string getComponentInfo() const;
	// Расширение кода объекта по компоненту - для перекраски танков
	virtual std::string getCodePostfix() const;
	// Получение порядка компонент при необходимости
	virtual int getOrder() const;
	// Обновление компонента по времени
	virtual void update(float dt);
};

