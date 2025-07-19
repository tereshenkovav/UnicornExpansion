#include "CppTools.h"

std::string clearStringFromEndl(const std::string& str) {
	std::string res = str;
	if (res.size() > 0) if (res[res.size() - 1] == 10) res.erase(res.end() - 1);
	if (res.size() > 0) if (res[res.size() - 1] == 13) res.erase(res.end() - 1);
	return res;
}

std::string trimString(const std::string & str) {
	auto start = str.begin();
	while (start != str.end() && std::isspace(*start)) ++start;
	auto end = str.end();
	do { --end; } while (end != start && std::isspace(*end));
	return std::string(start, end + 1);
}

std::string clearLineFromBOM(const std::string& str) {
	if (str.size() >= 3) {
		std::string BOM;
		BOM.push_back(239); BOM.push_back(187); BOM.push_back(191);
		if (str.substr(0, 3) == BOM) return str.substr(3); else return str;
	}
	else
		return str;
}

bool isStringNumber(const std::string& str) {
	for (int i = 0; i < str.size(); i++)
		if ((str[i] < '0') || (str[i] > '9'))
			return false;
	return true;
}

std::vector<std::string> splitString(const std::string& src, const std::string& delimiter) {
	std::string s = src;
	std::vector<std::string> tokens;
	size_t pos = 0;
	std::string token;
	while ((pos = s.find(delimiter)) != std::string::npos) {
		token = s.substr(0, pos);
		tokens.push_back(token);
		s.erase(0, pos + delimiter.length());
	}
	tokens.push_back(s);
	return tokens;
}

std::string replaceFirstString(std::string& str, const std::string & src, const std::string & dst) {
	std::size_t pos = str.find(src);
	if (pos == std::string::npos) return str;
	return str.replace(pos, src.length(), dst);
}

bool hasEnding (std::string const &fullString, std::string const &ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}
