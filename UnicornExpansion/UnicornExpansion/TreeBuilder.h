#pragma once

#include <vector>
#include <optional>
#include "Game.h"

enum TreeType { Bottom, BottomLeft, BottomRight, Top, TopLeft, TopRight, Left, Right,
  WaterTopLeft, WaterTop, WaterTopRight, WaterLeft, WaterRight, WaterBottomLeft, WaterBottom, WaterBottomRight,
  RoadHorz, RoadVert, RoadTopLeft, RoadTopRight, RoadBottomLeft, RoadBottomRight};

class TreeBuilder {
private:
	int width ;
	int height ;
	std::vector<std::vector<std::optional<TreeType>>> map ;
public:
	TreeBuilder() ;
	void updateByGame(const Game & game) ;
	std::optional<TreeType> getTreeType(int x, int y) const ;
} ;
