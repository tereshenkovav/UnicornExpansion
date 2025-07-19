// WaySearch.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <algorithm>

#include "WayFinder.h"
#include "GameUnit.h"

int _tmain(int argc, _TCHAR* argv[])
{

	const int W = 10 ;
	const int H = 10 ;
	BusyMap map ;
	map.setSize(W,H) ;
	map.reset() ;
map.setBusy(0,4) ;map.setBusy(1,4) ;	map.setBusy(2,4) ; map.setBusy(3,4) ; map.setBusy(4,4) ;
	map.setBusy(3,7) ; map.setBusy(4,7) ; map.setBusy(5,7) ; map.setBusy(6,7) ; map.setBusy(7,7) ; 

	map.setBusy(7,0) ; map.setBusy(7,1) ; map.setBusy(7,2) ; map.setBusy(7,3) ; map.setBusy(7,4) ; map.setBusy(7,5) ; map.setBusy(7,6) ;

	WayFinder finder(&map) ;
	
	Step ps = {2,1} ;
	Step pf = {9,2} ;

	auto way = finder.findWay(ps.x,ps.y,pf.x,pf.y) ;
	
	for (int j=0; j<W; j++) {
		for (int i=0; i<H; i++) {
			Step p = {i,j} ;			
			char c = map.isBusy(i,j)?'#':' ';
			if (std::find(way.begin(),way.end(),p)!=way.end()) c='+' ;
			if (p==ps) c='S' ;
			if (p==pf) c='F' ;
			std::cout<<c ;
		}
		std::cout<<std::endl ;
	}

	return 0;
}

