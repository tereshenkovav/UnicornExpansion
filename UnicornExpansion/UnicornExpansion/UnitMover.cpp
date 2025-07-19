#include "UnitMover.h"

UnitMover::UnitMover(BusyMap* busymap)
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
	
	if (busymap->isBusy(unit.getTarget().x,unit.getTarget().y)) {
		if (abs(unit.getXY().x - unit.getTarget().x)+
			abs(unit.getXY().y - unit.getTarget().y) == 1) {
			unit.resetTarget();// ����� ����������, �� ������� �������� � ������� �����
			return;
		}
	}

	WayFinder finder(busymap);
	auto way = finder.findWay(unit.getXY().x, unit.getXY().y, unit.getTarget().x,unit.getTarget().y);

	if (way.size() < 2) {
		// ���� �����������, ���� ����� � ������� �� ���������� ����(�� ������ �������, �� ����� �������� ��� ��������� �����)
		std::vector<Step> bestway;
		for (int i = 0; i < dxy.size(); i++) {
			way = finder.findWay(unit.getXY().x, unit.getXY().y, unit.getTarget().x + dxy[i].x, unit.getTarget().y + dxy[i].y);
			if (way.size() >= 2)
				if ((bestway.size() == 0) || (way.size() < bestway.size())) bestway = way;
		}
		way = bestway;
	}

	if (way.size() < 2) {
		unit.resetTarget();// ����� ����������, ���� �� �����
	}
	else {
		// ������ �������� �� ��, ��� �������� ������ �� 4 ������� ��� ����������
		auto newpos = way[way.size() - 2];
		if (newpos.x == unit.getXY().x) {
			if (newpos.y > unit.getXY().y) unit.moveTo(Moving::Down); else unit.moveTo(Moving::Up);
		}
		else {
			if (newpos.x > unit.getXY().x) unit.moveTo(Moving::Right); else unit.moveTo(Moving::Left);
		}
	}
}