#pragma once
#include "UnitComponent.h"

// Компонент, указывающий, что юнита нужно укрыть щитов из портала
class ComponentProtectable: public UnitComponent
{
public:
	using UnitComponent::UnitComponent;
};

