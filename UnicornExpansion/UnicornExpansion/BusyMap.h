#pragma once

#include <vector>

//  ласс дл€ хранени€ карты зан€тости - помогает определить, зан€та ли €чейка или нет
// ”добный двумерный массив булевых значений
class BusyMap {
private:
	int width ;
	int height ;
	std::vector<std::vector<bool>> map ;
public:
	BusyMap(int width, int height) ;
	void reset() ;
	void setBusy(int x, int y) ;
	bool isBusy(int x, int y) const ;
	int getWidth() const ;
	int getHeight() const ;
} ;
