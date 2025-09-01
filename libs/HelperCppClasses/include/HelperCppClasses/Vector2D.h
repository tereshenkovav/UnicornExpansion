#pragma once

#include <vector>

/*
   Удобный двумерный массив произвольных данных
   с установкой значения по умолчанию и контролем границ
*/
template<typename T>
class Vector2D {
private:
	int width ;
	int height ;
	T defoutboundvalue ;
	std::vector<std::vector<T>> matrix ;
public:
	Vector2D() {
	        defoutboundvalue = T() ;
		resize(0,0);
	}
	Vector2D(int width, int height) {
	        defoutboundvalue = T() ;
 		resize(width,height) ;
	}
	Vector2D(int width, int height, const T & value) {
	        defoutboundvalue = T() ;
 		resize(width,height) ;
 		fill(value) ;
	}
	void resize(int width, int height) {
		this->width = width ;
		this->height = height ;
		matrix.resize(width) ;
		for (int i=0; i<width; i++)
			matrix[i].resize(height) ;
	}
	void fill(const T & value) {
		for (int i=0; i<width; i++)
			for (int j=0; j<height; j++)
				matrix[i][j]=value ;
	}
	void resizeAndFill(int width, int height, const T & value) {
		resize(width,height) ;
		fill(value) ;
	}
	void setValue(int x, int y, const T & value) {
		if (x<0) return ;
		if (x>=width) return ;
		if (y<0) return ;
		if (y>=height) return ;
		matrix[x][y]=value ;
	}
	void setOutboundValue(const T & value) {
		defoutboundvalue = value ;
	}
	T getValue(int x, int y, const T & outboundvalue) const {
		if (x<0) return outboundvalue ;
		if (x>=width) return outboundvalue ;
		if (y<0) return outboundvalue ;
		if (y>=height) return outboundvalue ;
		return matrix[x][y] ;
	}
	T getValue(int x, int y) const {
		return getValue(x,y,defoutboundvalue) ;
	}
	int getWidth() const {
		return width ;
	}
	int getHeight() const {
		return height ;
	}
} ;
