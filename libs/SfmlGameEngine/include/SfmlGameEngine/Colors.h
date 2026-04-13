#pragma once

#include <string>
#include <map>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

namespace sfge {

// Класс цветов - хранит цвета с ключами
class Colors
{
private:
	std::map<std::string, sf::Color> colors;
public:
	// Загрузка из файла
	void loadFromFile(const std::string& filename);
	// Получение цвета по ключу
	sf::Color getColor(const std::string& name) const;
	sf::Color getColor(const std::string& name, sf::Color defcolor) const;
};

}
