#include "WayFinder.h"

WayFinder::WayFinder(Vector2D<bool> * map)
{
	this->map = map ;
	dists.resize(map->getWidth(),map->getHeight()) ;
	dists.setOutboundValue(-1) ;
	// Порядок здесь имеет значение, сначала идут прямые, потом диагональные
	dxy[0].x = 1;  dxy[0].y = 0;
	dxy[1].x = 0;  dxy[1].y = 1;
	dxy[2].x = 0;  dxy[2].y = -1;
	dxy[3].x = -1; dxy[3].y = 0;
	dxy[4].x = 1;  dxy[4].y = -1;
	dxy[5].x = 1;  dxy[5].y = 1;
	dxy[6].x = -1; dxy[6].y = 1;
	dxy[7].x = -1; dxy[7].y = -1;
}

std::vector<Step> WayFinder::fillWayByDists(int x, int y) const {
	int dist = dists.getValue(x,y) ;

	std::vector<Step> way ;
	
	Step step = {x,y} ;
	way.push_back(step) ;

	for (int w=dist-1; w>0; w--)
		for (int k=0; k<dxy.size(); k++) {
			int nx = x+dxy[k].x ;
			int ny = y+dxy[k].y ;
			if (dists.getValue(nx,ny)==w) {
				x=nx ;
				y=ny ;
				step.x = x ;
				step.y = y ;
				way.push_back(step) ;
				break;
			}
		}

	return way ;
}

std::vector<Step> WayFinder::findWay(int startx, int starty, int dstx, int dsty) {
	dists.fill(0) ;
	std::vector<Step> way ;

	int tekdist = 1 ;
	dists.setValue(startx,starty,tekdist) ;

	// Алгоритм на каждой итерации обходит всю карту, но можно реализовать через запись в векторе последней волны обхода
	while (true) {
		int cntfilled = 0 ;
		for (int i=0; i<map->getWidth(); i++) 
			for (int j=0; j<map->getHeight(); j++)
				if (dists.getValue(i,j)==tekdist)
					for (int k=0; k<dxy.size(); k++) {
						int nx = i+dxy[k].x ;
						int ny = j+dxy[k].y ;
						if ((dists.getValue(nx,ny)==0)&&(!map->getValue(nx,ny))) {
							dists.setValue(nx,ny,tekdist+1) ;
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
