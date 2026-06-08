#include "SfmlGameEngine/Languages.h"
#include <fstream>
#include "HelperCppClasses/StringTools.h"

namespace sfge {

Languages::Languages() {
	all.push_back("ru");
	upper.push_back("RU");
	tek = 0;
}

void Languages::loadFromFile(const std::string& filename)
{
	all.clear();
	upper.clear();

	std::string line;
	std::ifstream fin(filename);
	if (!fin.is_open()) return ;

	while (std::getline(fin, line))
	{
		line = trimString(clearLineFromBOM(clearStringFromEndl(line)));
		if (line.size() == 0) continue;
		auto lines = splitString(line, "=");
		all.push_back(lines[0]);
		upper.push_back(lines[1]);		
	}
	fin.close();
}

void Languages::setCurrentByFile(const std::string& filename) {
	std::string line;
	std::ifstream fin(filename);
	if (!fin.is_open()) return;

	if (std::getline(fin, line)) setCurrentByValue(trimString(clearLineFromBOM(clearStringFromEndl(line))));
	fin.close();
}

void Languages::setCurrentByValue(const std::string& value)
{
	for (int i = 0; i < all.size(); i++)
		if (all[i] == value) tek = i;
}

void Languages::switchCurrent() {
	tek++;
	if (tek >= all.size()) tek = 0;
}

std::string Languages::getCurrent() const {
	return all[tek];
}

std::string Languages::getCurrentUpper() const
{
	return upper[tek];
}

std::string Languages::getFileNameByLangIfExist(const std::string& filename) {
	std::filesystem::path fi(filename);
	auto ext = fi.extension().generic_string();
	auto langfile = fi.replace_extension(getCurrent()+ext);
	if (std::filesystem::exists(langfile)) return langfile.generic_string(); else return filename;
}

}