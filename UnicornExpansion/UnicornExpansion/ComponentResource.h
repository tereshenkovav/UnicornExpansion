#pragma once
#include "UnitComponent.h"

// ��������� ������� - ��� ����� ��������
class ComponentResource: public UnitComponent
{
public:
	using UnitComponent::UnitComponent;
	virtual std::string getComponentInfo() const;
};

