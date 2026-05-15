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
	
	way.push_back({x,y});

	for (int w=dist-1; w>0; w--)
		for (int k=0; k<dxy.size(); k++) {
			int nx = x+dxy[k].x ;
			int ny = y+dxy[k].y ;
			if (dists.getValue(nx,ny)==w) {
				x=nx ;
				y=ny ;
				way.push_back({ x,y });
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

	std::vector<Step> wave;
	std::vector<Step> newwave;
	newwave.reserve(2 * (dists.getWidth() + dists.getHeight()));
	wave.push_back({ startx,starty });

	while (true) {
		for (int i = 0; i < wave.size(); i++)
					for (int k=0; k<dxy.size(); k++) {
						int nx = wave[i].x + dxy[k].x ;
						int ny = wave[i].y + dxy[k].y ;
						if ((dists.getValue(nx,ny)==0)&&(!map->getValue(nx,ny))) {
							dists.setValue(nx,ny,tekdist+1) ;
							newwave.push_back({ nx,ny });
							if ((nx==dstx)&&(ny==dsty)) return fillWayByDists(nx,ny) ;
						}
					}
		if (newwave.size() == 0) return way;
		wave = std::move(newwave);
		newwave = std::vector<Step>();
		tekdist++ ;
	}	
}

WayFinder::~WayFinder(void)
{
}
