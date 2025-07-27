#pragma once
#include "UnitComponent.h"

// ��������� ������� - ���� ����� ������������� ������ �����
class ComponentHealer : public UnitComponent
{
private:
	int heal_rate;
	int tek_upgrade_pos;
	float locked_left;
	bool isactive;
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
	virtual void update(float dt);
	virtual int getOrder() const;
	bool isActive() const;
	void setActive(bool value);
	void LockTemporary();
	bool isLocked() const;
};

