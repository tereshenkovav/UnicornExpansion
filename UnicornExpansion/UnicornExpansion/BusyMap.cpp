#include "BusyMap.h"

BusyMap::BusyMap(int width, int height) {
	this->width = width ;
	this->height = height ;
	map.resize(width) ;
	for (int i=0; i<width; i++)
		map[i].resize(height) ;
	reset() ;
}

void BusyMap::reset() {
	for (int i=0; i<width; i++)
		for (int j=0; j<height; j++)
			map[i][j]=false ;
}

void BusyMap::setBusy(int x, int y) {
	if (x<0) return ;
	if (x>=width) return ;
	if (y<0) return ;
	if (y>=height) return ;
	map[x][y]=true ;
}

bool BusyMap::isBusy(int x, int y) const {
	if (x<0) return true;
	if (x>=width) return true;
	if (y<0) return true;
	if (y>=height) return true;	
	return map[x][y] ;
}

int BusyMap::getWidth() const
{
	return width;
}

int BusyMap::getHeight() const
{
	return height;
}
