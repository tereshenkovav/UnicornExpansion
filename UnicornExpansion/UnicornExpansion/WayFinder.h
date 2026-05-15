#pragma once

#include "HelperCppClasses/Vector2D.h"
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
	Vector2D<bool>* map ;
	Vector2D<int> dists ;
	std::array<Step,8> dxy ;
	std::vector<Step> fillWayByDists(int x, int y) const ;
public:
	// Создаем класс
	WayFinder(Vector2D<bool> * map);
	// Ищем путь от и до
	std::vector<Step> findWay(int startx, int starty, int dstx, int dsty) ;
	~WayFinder(void);
};

