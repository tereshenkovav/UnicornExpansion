#include "TreeBuilder.h"

TreeBuilder::TreeBuilder() {
}

void TreeBuilder::updateByGame(const Game & game) {
	this->width = game.getWidth() ;
	this->height = game.getHeight() ;
	map.resize(width) ;
	for (int i=0; i<width; i++)
		map[i].resize(height) ;
	for (int i=0; i<width; i++)
		for (int j = 0; j < height; j++) {
			map[i][j] = std::nullopt;
			if (game.getMap(i, j) == Terrain::Ground) {
				if (game.getMap(i, j - 1) == Terrain::Forest) {
					if (game.getMap(i + 1, j) == Terrain::Forest)
						map[i][j] = TreeType::BottomLeft;
					else
					if (game.getMap(i - 1, j) == Terrain::Forest)
						map[i][j] = TreeType::BottomRight;
					else
						map[i][j] = TreeType::Bottom;
				}
				else
				if (game.getMap(i, j + 1) == Terrain::Forest) {
					if (game.getMap(i + 1, j) == Terrain::Forest)
						map[i][j] = TreeType::TopLeft;
					else
					if (game.getMap(i - 1, j) == Terrain::Forest)
						map[i][j] = TreeType::TopRight;
					else
						map[i][j] = TreeType::Top;
				}
				else
				if (game.getMap(i + 1, j) == Terrain::Forest) {
					map[i][j] = TreeType::Left;
				}
				else
				if (game.getMap(i - 1, j) == Terrain::Forest) {
					map[i][j] = TreeType::Right;
				}
			}
			if (game.getMap(i, j) == Terrain::Water) {
				bool gleft = (game.getMap(i - 1, j) == Terrain::Ground) && (i > 0);
				bool gright = (game.getMap(i + 1, j) == Terrain::Ground) && (i < game.getWidth() - 1);
				bool gtop = (game.getMap(i, j - 1) == Terrain::Ground) && (j > 0);
				bool gbottom = (game.getMap(i, j + 1) == Terrain::Ground) && (j < game.getHeight() - 1);
				if (gtop) {
					if (gleft) map[i][j] = TreeType::WaterTopLeft; else
					if (gright) map[i][j] = TreeType::WaterTopRight; else
						map[i][j] = TreeType::WaterTop;
				}
				else
				if (gbottom) {
					if (gleft) map[i][j] = TreeType::WaterBottomLeft; else
					if (gright) map[i][j] = TreeType::WaterBottomRight; else
						map[i][j] = TreeType::WaterBottom;
				}
				else
				if (gleft) map[i][j] = TreeType::WaterLeft;
				else
				if (gright) map[i][j] = TreeType::WaterRight;
			}
			if (game.getMap(i, j) == Terrain::Road) {
				bool rleft = (game.getMap(i - 1, j) == Terrain::Road) || (i == 0);
				bool rright = (game.getMap(i + 1, j) == Terrain::Road) || (i == game.getWidth() - 1);
				bool rtop = (game.getMap(i, j - 1) == Terrain::Road) || (j == 0);
				bool rbottom = (game.getMap(i, j + 1) == Terrain::Road) || (j == game.getHeight() - 1);
				if ((rleft ? 1 : 0) + (rright ? 1 : 0) + (rtop ? 1 : 0) + (rbottom ? 1 : 0) == 2) {
					if (rleft && rright) map[i][j] = TreeType::RoadHorz; else
					if (rtop && rbottom) map[i][j] = TreeType::RoadVert; else
					if (rtop && rleft) map[i][j] = TreeType::RoadTopLeft; else
					if (rtop && rright) map[i][j] = TreeType::RoadTopRight; else
					if (rbottom && rleft) map[i][j] = TreeType::RoadBottomLeft; else
					if (rbottom && rright) map[i][j] = TreeType::RoadBottomRight;
				}
			}
		}
}

std::optional<TreeType> TreeBuilder::getTreeType(int x, int y) const {
	if (x<0) return std::nullopt;
	if (x>=width) return std::nullopt;
	if (y<0) return std::nullopt;
	if (y>=height) return std::nullopt;
	return map[x][y] ;
}
