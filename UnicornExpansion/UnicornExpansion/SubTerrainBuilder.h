#pragma once

#include <vector>
#include <optional>
#include "Game.h"

enum TerrainSubType { TreeBottom, TreeBottomLeft, TreeBottomRight, TreeTop, TreeTopLeft, TreeTopRight, TreeLeft, TreeRight,
  WaterTopLeft, WaterTop, WaterTopRight, WaterLeft, WaterRight, WaterBottomLeft, WaterBottom, WaterBottomRight,
  RoadHorz, RoadVert, RoadTopLeft, RoadTopRight, RoadBottomLeft, RoadBottomRight};

class SubTerrainBuilder {
private:
	int width ;
	int height ;
	std::vector<std::vector<std::optional<TerrainSubType>>> map ;
public:
	SubTerrainBuilder() ;
	void updateByGame(const Game & game) ;
	std::optional<TerrainSubType> getTerrainSubType(int x, int y) const ;
} ;
