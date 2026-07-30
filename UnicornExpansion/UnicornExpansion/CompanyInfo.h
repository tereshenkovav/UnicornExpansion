#pragma once
#include <string>

class CompanyInfo
{
private:
	std::string company;
	std::string lang;
	int levelcount;
public:
	CompanyInfo(const std::string& company, const std::string& lang);
	int getLevelCount() const;
	std::string getLevelStringsFileName(int leveln) const;
	std::string getLevelMapFileName(int leveln) const;
	std::string getLevelScriptFileName(int leveln) const;
	std::string getNameFileName() const;
	std::string getDescrFileName() const;
	std::string getPostfixFileName() const;
	// Небольшой блок жестко закодированных параметров для встроенных кампаний
	// Доступны все карты на старте
	bool isAllowedAllMaps() const;
	// У кампании нет меню - после завершения игры идти в главное меню всегда
	bool isNoCompanyMenu() const;
	// У кампании нет последовательности - после победы возвращаться в меню кампании вместо следующего уровня или постфикса
	bool isNoCompanySequence() const;
};

