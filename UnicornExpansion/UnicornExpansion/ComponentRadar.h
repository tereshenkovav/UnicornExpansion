#pragma once
#include "UnitComponent.h"

// ��������� ������ - ����� �������� ����� �� ������� ����������
class ComponentRadar : public UnitComponent
{
private:
public:
	using UnitComponent::UnitComponent;
	virtual std::vector<UnitAction> getActions() const;
	virtual bool applyAction(const UnitAction& action);
	virtual std::string getComponentInfo() const;
	virtual std::string getCodePostfix() const;
	virtual int getOrder() const;
};


