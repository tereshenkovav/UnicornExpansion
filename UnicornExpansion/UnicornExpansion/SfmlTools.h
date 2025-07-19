#pragma once

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

// Класс утилит для SFML
class SfmlTools
{
public:
	// Перевод строки UTF8 в текст для SFML
	static sf::String utf2text(const std::string& str);
	// Получение цвета с изменением яркости от 0 и выше
	static sf::Color getColorAsBright(const sf::Color& color, float bright);
};

