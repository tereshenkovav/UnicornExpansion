#pragma once

#include <string>
#include <map>
#include <SFML/System.hpp>

namespace sfge {

// Класс языков
class Languages
{
private:
	std::vector<std::string> all;
	std::vector<std::string> upper;
	int tek;
public:
	Languages();
	// Загрузка языков из файла
	void loadFromFile(const std::string& filename);
	void setCurrentByFile(const std::string& filename);
	void setCurrentByValue(const std::string& value);
	void switchCurrent();
	std::string getCurrent() const;
	std::string getCurrentUpper() const;
	std::string getFileNameByLangIfExist(const std::string& filename);
};

}