#pragma once

#include <vector>
#include <string>

class UnitComponent;
class Game;

// ������ ��� �������� - ��� ���, �������, ������� � �����, � ����� ������ �� ���������, ��� �������� ���������
struct UnitAction {
	std::string code;
	std::string caption;
	int energy;
	int time;
	UnitComponent* component;
};

// ����� ���������� - �� ���� ������� ������.
// ���������� ������ �������� ������ ���� ��� �������� ������������ ����� ������, �������� ������� ����������� ������.
class UnitComponent
{
protected:
	Game * game;
	int unit_id;
public:
	// �������� ���������� � �����
	UnitComponent(Game * game);
	// ���������� ����, � ������� ��������� ���������
	void setUnitID(int unit_id);
	virtual ~UnitComponent();
	// ����������� ������ ��� ���������� � ��������, �� ��������� ��� ������ ������� ����
	// ��������
	virtual std::vector<UnitAction> getActions() const;
	// ���������� �������� - � ������ ���������� � � ������ ������
	virtual bool applyAction(const UnitAction& action);
	virtual bool canApplyAction(const UnitAction& action, std::string* msgcode) const;
	virtual bool doPrefixAction(const UnitAction& action);
	// ��������� ������� ����������
	virtual std::string getComponentInfo() const;
	// ���������� ���� ������� �� ���������� - ��� ���������� ������
	virtual std::string getCodePostfix() const;
	// ��������� ������� ��������� ��� �������������
	virtual int getOrder() const;
	// ���������� ���������� �� �������
	virtual void update(float dt);
};

