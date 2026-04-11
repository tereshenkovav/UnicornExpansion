#include "SfmlGameEngine/Texts.h"
#include "SfmlGameEngine/SfmlTools.h"
#include <fstream>
#include "HelperCppClasses/StringTools.h"

namespace sfge {

std::string prepLine1(const std::string& str) {
	return trimString(clearLineFromBOM(clearStringFromEndl(str)));
}

void Texts::loadFromFile(const std::string& filename)
{
	strings.clear();

	std::string line;
	std::ifstream fin(filename);
	if (!fin.is_open()) return ;

	while (std::getline(fin, line))
	{
		line = prepLine1(line);
		if (line.size() == 0) continue;
		auto lines = splitString(line, "=");
		if (lines.size() >= 2) strings[lines[0]] = replaceAllString(lines[1], "\\n", "\n");
	}
	fin.close();
}

std::string Texts::getStr(const std::string& name) const
{
	if (strings.count(name) > 0) return strings.at(name); else return "Unknown key: " + name;
}

sf::String Texts::getSfmlStr(const std::string& name) const
{
	return sfge::SfmlTools::utf2text(getStr(name));
}

sf::String Texts::getSfmlStrReplacedConsts(const std::string & str) const
{
	std::string newstr = str;
	for (const auto& pair : strings)
		newstr = replaceFirstString(newstr, "$" + pair.first + "$", pair.second);
	return sfge::SfmlTools::utf2text(newstr);
}

}
