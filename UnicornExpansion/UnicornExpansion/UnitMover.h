#pragma once

#include <vector>
#include <string>
#include <HelperCppClasses\Vector2D.h>
#include "GameUnit.h"
#include "WayFinder.h"

//  ласс дл€ движени€ - имеет ссылку на блок зан€тости €чеек и обновл€ет юнит по мере выполнени€ движени€
class UnitMover {
private:
	Vector2D<bool> * busymap ;
	std::array<Step, 8> dxy;
public:
	UnitMover(Vector2D<bool> * busymap);
	void updateUnit(GameUnit & unit) const ;
} ;
