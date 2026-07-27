#pragma once
#include "UnitComponent.h"

//  омпонент, определ€ющий способность двигатьс€
// ѕросто признака скорости недостаточно, иначе можно будет двигать врагов
class ComponentMovable: public UnitComponent
{
public:
	using UnitComponent::UnitComponent;
};

