#pragma once

#include <vector>
#include <string>
#include "BusyMap.h"
#include "GameUnit.h"
#include "WayFinder.h"

//  ласс дл€ движени€ - имеет ссылку на блок зан€тости €чеек и обновл€ет юнит по мере выполнени€ движени€
class UnitMover {
private:
	BusyMap * busymap ;
	std::array<Step, 8> dxy;
public:
	UnitMover(BusyMap* busymap);
	void updateUnit(GameUnit & unit) const ;
} ;
