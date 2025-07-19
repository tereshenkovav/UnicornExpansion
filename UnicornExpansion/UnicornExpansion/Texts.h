#pragma once

#include <string>
#include <map>
#include <SFML/System.hpp>

// ����� ������� - ������ ������ � �������
class Texts
{
private:
	std::map<std::string, std::string> strings;
public:
	// �������� ������� �� �����
	void loadFromFile(const std::string& filename);
	// ��������� ������ �� �����
	std::string getStr(const std::string& name);
	// ��������� ����� ������ SFML �� �����
	sf::String getSfmlStr(const std::string& name);
	// ��������� ������ SFML � ������� ���� ������ � ������ �� �� ��������
	sf::String getSfmlStrReplacedConsts(const std::string & str);
};

