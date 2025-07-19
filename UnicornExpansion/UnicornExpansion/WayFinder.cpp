#include "WayFinder.h"

WayFinder::WayFinder(BusyMap * map)
{
	this->map = map ;
	dists.resize(map->getWidth()) ;
	for (int i=0; i<map->getWidth(); i++)
		dists[i].resize(map->getHeight()) ;
	dxy[0].x=1 ; dxy[0].y=0 ;
	dxy[1].x=-1 ; dxy[1].y=0 ;
	dxy[2].x=0 ; dxy[2].y=1 ;
	dxy[3].x=0 ; dxy[3].y=-1 ;
}

bool WayFinder::isXYInMap(int x, int y) const {
	if (x<0) return false ;
	if (x>=map->getWidth()) return false ;
	if (y<0) return false ;
	if (y>=map->getHeight()) return false ;
	return true ;
}
						
void WayFinder::clearDists() {
	for (int i=0; i<map->getWidth(); i++)
		for (int j=0; j<map->getHeight(); j++)
			dists[i][j]=0 ;
}

std::vector<Step> WayFinder::fillWayByDists(int x, int y) const {
	int dist = dists[x][y] ;

	std::vector<Step> way ;
	
	Step step = {x,y} ;
	way.push_back(step) ;

	for (int w=dist-1; w>0; w--)
		for (int k=0; k<dxy.size(); k++) {
			int nx = x+dxy[k].x ;
			int ny = y+dxy[k].y ;
			if (isXYInMap(nx,ny))
				if (dists[nx][ny]==w) {
					x=nx ;
					y=ny ;
					step.x = x ;
					step.y = y ;
					way.push_back(step) ;
				}
		}
	return way ;
}

std::vector<Step> WayFinder::findWay(int startx, int starty, int dstx, int dsty) {
	clearDists();
	std::vector<Step> way ;
	
	int tekdist = 1 ;
	dists[startx][starty]=tekdist ;

	// Алгоритм на каждой итерации обходит всю карту, но можно реализовать через запись в векторе последней волны обхода
	while (true) {
		int cntfilled = 0 ;
		for (int i=0; i<map->getWidth(); i++) 
			for (int j=0; j<map->getHeight(); j++)
				if (dists[i][j]==tekdist)
					for (int k=0; k<dxy.size(); k++) {
						int nx = i+dxy[k].x ;
						int ny = j+dxy[k].y ;
						if (isXYInMap(nx,ny))
							if ((dists[nx][ny]==0)&&(!map->isBusy(nx,ny))) {
								dists[nx][ny]=tekdist+1 ;
								cntfilled++ ;
								if ((nx==dstx)&&(ny==dsty)) return fillWayByDists(nx,ny) ;
							}
					}
		if (cntfilled==0) return way ;
		tekdist++ ;
	}	
}

WayFinder::~WayFinder(void)
{
}
