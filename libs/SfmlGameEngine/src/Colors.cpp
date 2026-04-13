#include "SfmlGameEngine/Colors.h"
#include "SfmlGameEngine/SfmlTools.h"
#include <fstream>
#include "HelperCppClasses/StringTools.h"

namespace sfge {

void Colors::loadFromFile(const std::string& filename)
{
	colors.clear();

	std::string line;
	std::ifstream fin(filename);
	if (!fin.is_open()) return ;

	while (std::getline(fin, line))
	{
		line = trimString(clearLineFromBOM(clearStringFromEndl(line)));
		if (line.size() == 0) continue;
		auto lines = splitString(line, "=");
		if (lines.size() >= 2) {
			auto rgb = splitString(lines[1], ",");
			if (rgb.size() >= 3) {
				sf::Color c;
				c.r = std::stoi(trimString(rgb[0]));
				c.g = std::stoi(trimString(rgb[1]));
				c.b = std::stoi(trimString(rgb[2]));
				c.a = (rgb.size() == 4)?std::stoi(trimString(rgb[3])):255;
				colors[trimString(lines[0])] = c;
			}
		}
	}
	fin.close();
}

sf::Color Colors::getColor(const std::string& name) const
{
	return getColor(name,sf::Color::White) ;
}

sf::Color Colors::getColor(const std::string& name, sf::Color defcolor) const
{
	if (colors.count(name) > 0) return colors.at(name); else return defcolor;
}

}
