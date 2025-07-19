#pragma once

#include <string>
#include <vector>

// Процедуры для работы со строками
// Очистка от символов конца строки
std::string clearStringFromEndl(const std::string& str);
// Обрезка строки от пробелов
std::string trimString(const std::string& str);
// Очистка строки от BOM
std::string clearLineFromBOM(const std::string& str);
// Проверка, является ли строка числом
bool isStringNumber(const std::string& str);
// Разделение строки
std::vector<std::string> splitString(const std::string& src, const std::string& delimiter);
// Замена первой подстроки
std::string replaceFirstString(std::string& str, const std::string& src, const std::string& dst);
// Проверка завершения одной строки другой
bool hasEnding (std::string const &fullString, std::string const &ending) ;
