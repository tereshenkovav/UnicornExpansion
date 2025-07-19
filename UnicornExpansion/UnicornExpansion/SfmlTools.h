#pragma once

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

// ����� ������ ��� SFML
class SfmlTools
{
public:
	// ������� ������ UTF8 � ����� ��� SFML
	static sf::String utf2text(const std::string& str);
	// ��������� ����� � ���������� ������� �� 0 � ����
	static sf::Color getColorAsBright(const sf::Color& color, float bright);
};

