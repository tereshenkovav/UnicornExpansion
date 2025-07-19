#pragma once
#include "UnitComponent.h"

// ��������� ������� - ���� ����� ������������� ������ �����
class ComponentHealer : public UnitComponent
{
private:
	int heal_rate;
	int tek_upgrade_pos;
public:
	ComponentHealer(Game* game);
	// ��������, ��������� � ������ ������� �� ������
	int getHealerRate() const;
	int getHealerDistance() const;
	int getHealerEnergyCost() const;
	virtual std::vector<UnitAction> getActions() const;
	virtual bool applyAction(const UnitAction& action);
	virtual std::string getComponentInfo() const;
	virtual std::string getCodePostfix() const;
	virtual int getOrder() const;
};

