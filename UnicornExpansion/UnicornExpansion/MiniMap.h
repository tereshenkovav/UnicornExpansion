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
	float rect_w;
	float rect_h;
public:
	MiniMap();
	// ��������� ������� ����-����� - ������� ������ ������� � ��������� ���� ��� �����������, ������� ������ ������� ����� (������)
    void prepareMiniMap(int px, int py, int renderw, int renderh, int worldw, int worldh,
		int windoww, int windowh, int fullw, int fullh);
	// ���������� ���� ����� �����
	void setCellColor(int i, int j, const sf::Color & c);
	// ��������� ������ ������
	void setCenter(int cx, int cy);
	// ����� ����� � ������
	void drawTo(sf::RenderTarget* target) const;
};

