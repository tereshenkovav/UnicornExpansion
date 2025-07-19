#pragma once

namespace sf {

	// ����� ��� ��������� �������
class Countdown
{
private:
	float left ;
	bool oncereach ;
public:
	Countdown(void);
	// ��������� �������
	void upset(float interval) ;
	// ���������� �������
	void update(float dt) ;
	// ��������, ���� �� ������
	bool isActive() ;
	// ������� ��� ������ ������ ����� ���������� ������� ������ ������� ����������, ����� false
	bool onceReachNol() ;
	void reset() ;
	~Countdown(void);
};

}
