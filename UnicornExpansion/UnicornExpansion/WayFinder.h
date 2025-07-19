#pragma once

#include "BusyMap.h"
#include <vector>
#include <array>

// Запись хода - куда идем и метод сравнения.
struct Step {
	int x ;
	int y ;
	bool operator == (const Step &step) const {
		return this->x == step.x && this->y == step.y ;
	}
} ;

// Класс поиска пути - очень полезен для обхода ячеек и поиска, как добраться по сложной местности.
// Работает методом заливки, от старта и пока не достигли назначения
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
	// Создаем класс
	WayFinder(BusyMap * map);
	// Ищем путь от и до
	std::vector<Step> findWay(int startx, int starty, int dstx, int dsty) ;
	~WayFinder(void);
};

