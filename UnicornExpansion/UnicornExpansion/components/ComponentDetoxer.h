#pragma once
#include "UnitComponent.h"

// Компонент, определяющий действие очистки грибов
class ComponentDetoxer : public UnitComponent
{
private:
	int detox_value;
public:
	ComponentDetoxer(Game* game);
	virtual std::string getComponentInfo() const;
	// Длина и сила очистки
	int getDetoxValue() const;
	int getDetoxDistance() const;
	virtual std::string getCodePostfix() const;
	virtual int getOrder() const;
};
