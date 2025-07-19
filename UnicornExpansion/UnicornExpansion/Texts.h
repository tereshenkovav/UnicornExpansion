#pragma once

#include <string>
#include <map>
#include <SFML/System.hpp>

// Класс текстов - хранит строки с ключами
class Texts
{
private:
	std::map<std::string, std::string> strings;
public:
	// Загрузка текстов из файла
	void loadFromFile(const std::string& filename);
	// Получение строки по ключу
	std::string getStr(const std::string& name);
	// Получение сразу строки SFML по ключу
	sf::String getSfmlStr(const std::string& name);
	// Получение строки SFML с заменой всех ключей в строке на их значения
	sf::String getSfmlStrReplacedConsts(const std::string & str);
};

