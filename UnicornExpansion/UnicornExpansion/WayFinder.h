#pragma once

#include "BusyMap.h"
#include <vector>
#include <array>

// ������ ���� - ���� ���� � ����� ���������.
struct Step {
	int x ;
	int y ;
	bool operator == (const Step &step) const {
		return this->x == step.x && this->y == step.y ;
	}
} ;

// ����� ������ ���� - ����� ������� ��� ������ ����� � ������, ��� ��������� �� ������� ���������.
// �������� ������� �������, �� ������ � ���� �� �������� ����������
class WayFinder
{
private:
	BusyMap * map ;
	std::vector<std::vector<int>> dists ;
	std::array<Step,4> dxy ;
	void clearDists() ;
	std::vector<Step> fillWayByDists(int x, int y) const ;
	bool isXYInMap(int x, int y) const ;
public:
	// ������� �����
	WayFinder(BusyMap * map);
	// ���� ���� �� � ��
	std::vector<Step> findWay(int startx, int starty, int dstx, int dsty) ;
	~WayFinder(void);
};

