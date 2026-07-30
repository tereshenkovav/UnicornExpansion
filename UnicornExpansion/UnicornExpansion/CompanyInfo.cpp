#include "CompanyInfo.h"
#include <filesystem>
#include <format>

CompanyInfo::CompanyInfo(const std::string& company, const std::string& lang) {
	this->company = company;
	this->lang = lang;
	for (int i = 0; ; i++)
		if (!std::filesystem::exists(std::format("company/{}/level{}.map", company, i))) {
			levelcount = i;
			break;
		}
}
int CompanyInfo::getLevelCount() const {
	return levelcount;
}

std::string CompanyInfo::getLevelStringsFileName(int leveln) const
{
	return std::format("company/{}/level{}.{}.strings", company, leveln, lang);
}

std::string CompanyInfo::getLevelMapFileName(int leveln) const
{
	return std::format("company/{}/level{}.map", company, leveln);
}

std::string CompanyInfo::getLevelScriptFileName(int leveln) const
{
	return std::format("company/{}/level{}.script", company, leveln);
}

std::string CompanyInfo::getNameFileName() const {
	return std::format("company/{}/name.{}.txt", company, lang);
}
std::string CompanyInfo::getDescrFileName() const {
	return std::format("company/{}/descr.{}.txt", company, lang);
}
std::string CompanyInfo::getPostfixFileName() const {
	return std::format("company/{}/postfix.{}.txt", company, lang);
}

bool CompanyInfo::isAllowedAllMaps() const {
	// Карты испытаний всегда открыты
	return company == "challenge";
}

bool CompanyInfo::isNoCompanyMenu() const {
	// Обучение возвращаем в главное меню
	return company == "tutorial";
}

bool CompanyInfo::isNoCompanySequence() const {
	// Для испытаний не нужно переключаться к следующей карте
	return company == "challenge";
}
