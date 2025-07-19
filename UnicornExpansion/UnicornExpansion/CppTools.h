#pragma once

#include <string>
#include <vector>

// ��������� ��� ������ �� ��������
// ������� �� �������� ����� ������
std::string clearStringFromEndl(const std::string& str);
// ������� ������ �� ��������
std::string trimString(const std::string& str);
// ������� ������ �� BOM
std::string clearLineFromBOM(const std::string& str);
// ��������, �������� �� ������ ������
bool isStringNumber(const std::string& str);
// ���������� ������
std::vector<std::string> splitString(const std::string& src, const std::string& delimiter);
// ������ ������ ���������
std::string replaceFirstString(std::string& str, const std::string& src, const std::string& dst);
// �������� ���������� ����� ������ ������
bool hasEnding (std::string const &fullString, std::string const &ending) ;
