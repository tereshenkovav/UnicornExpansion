#pragma once

#include <SFML/Graphics.hpp>

// ����� ����-����� ��� �����������
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
	// ����������� ������������
	float kx;
	float ky;
	float scalex;
	float scaley;
	float scale;
	float rect_w;
	float rect_h;
public:
	MiniMap();
	// ��������� ������� ����-����� - ������� ������ ������� � ��������� ���� ��� �����������, ������� ������ ������� ����� (������)
    void prepareMiniMap(int px, int py, int renderw, int renderh, int worldw, int worldh,
		int windoww, int windowh, int fullw, int fullh);
	void setWindowSize(int windoww, int windowh);
	// ���������� ���� ����� �����
	void setCellColor(int i, int j, const sf::Color & c);
	// ��������� ������ ������
	void setCenter(int cx, int cy);
	sf::Vector2f getWorldPosByMapPos(float mx, float my) const;
	bool isXYonMap(sf::Vector2i pos) const;
	// ����� ����� � ������
	void drawTo(sf::RenderTarget* target) const;
};

