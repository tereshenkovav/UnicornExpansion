#pragma once

#include <SFML/Graphics.hpp>

// Класс мини-карты для отображения
class MiniMap
{
private:
	sf::VertexArray minimap;
	sf::RectangleShape minimap_current;
	int x;
	int y;
	int w;
	int h;
	int worldw;
	int worldh;
	int windoww;
	int windowh;
	int fullw;
	int fullh;
	// Вычисляемые коэффициенты
	float kx;
	float ky;
	float rect_w;
	float rect_h;
public:
	MiniMap();
	// Установка свойств мини-карту - размеры самого рендера и параметры мира для отображения, включая размер видимой части (камеры)
    void prepareMiniMap(int px, int py, int renderw, int renderh, int worldw, int worldh,
		int windoww, int windowh, int fullw, int fullh);
	// Установить цвет точки карты
	void setCellColor(int i, int j, const sf::Color & c);
	// Установка центра камеры
	void setCenter(int cx, int cy);
	// Вывод карты в рендер
	void drawTo(sf::RenderTarget* target) const;
};

