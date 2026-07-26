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
};

