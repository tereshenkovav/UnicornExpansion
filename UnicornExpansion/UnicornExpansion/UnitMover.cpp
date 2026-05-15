#include "UnitMover.h"

UnitMover::UnitMover(Vector2D<bool>* busymap)
{
	dxy[0] = { -1, -1 };
	dxy[1] = { 0, -1 };
	dxy[2] = { 1, -1 };
	dxy[3] = { -1,  0 };
	dxy[4] = { 1,  0 };
	dxy[5] = { -1,  1 };
	dxy[6] = { 0,  1 };
	dxy[7] = { 1,  1 };

	this->busymap = busymap;
}

void UnitMover::updateUnit(GameUnit & unit) const {
	if (!unit.isTargeted()) return ;
	if (unit.isMoving()) return ;
	
	if (busymap->getValue(unit.getTarget().x, unit.getTarget().y)) {
		for (int i = 0; i < dxy.size(); i++)
			if ((unit.getXY().x + dxy[i].x == unit.getTarget().x) &&
				(unit.getXY().y + dxy[i].y == unit.getTarget().y)) {
				unit.resetTarget();// Здесь сбрасываем, мы подошли вплотную к занятой точке в зоне dxy
				return;
			}
	}

	WayFinder finder(busymap);
	auto way = finder.findWay(unit.getXY().x, unit.getXY().y, unit.getTarget().x,unit.getTarget().y);

	if (way.size() < 2) {
		// Цель недостижима, ищем рядом в клетках без выбора оптимального
		for (int i = 0; i < dxy.size(); i++) {
			way = finder.findWay(unit.getXY().x, unit.getXY().y, unit.getTarget().x + dxy[i].x, unit.getTarget().y + dxy[i].y);
			if (way.size() >= 2) break;
		}
	}

	if (way.size() < 2) {
		unit.resetTarget();// Здесь сбрасываем, пока не нашли
	}
	else {
		// Движение на все 8 направлений
		auto newpos = way[way.size() - 2];

		if (newpos.x < unit.getXY().x) {
			if (newpos.y < unit.getXY().y) unit.moveTo(Moving::LeftUp); else
			if (newpos.y > unit.getXY().y) unit.moveTo(Moving::LeftDown); else
			unit.moveTo(Moving::Left);
		}
		else
		if (newpos.x > unit.getXY().x) {
			if (newpos.y < unit.getXY().y) unit.moveTo(Moving::RightUp); else
			if (newpos.y > unit.getXY().y) unit.moveTo(Moving::RightDown); else
			unit.moveTo(Moving::Right);
		}
		else {
			if (newpos.y < unit.getXY().y) unit.moveTo(Moving::Up); else unit.moveTo(Moving::Down);
		}
	}
}