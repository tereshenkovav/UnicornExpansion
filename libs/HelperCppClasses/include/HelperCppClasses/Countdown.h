#pragma once

// Класс для обратного отсчета
class Countdown
{
private:
	float left ;
	bool oncereach ;
public:
	Countdown();
	// Установка отсчета
	void upset(float interval) ;
	// Обновление отсчета
	void update(float dt) ;
	// Проверка, идет ли отсчет
	bool isActive() const ;
	// Функция при первом вызове после завершения отсчета вернет признак завершения, иначе false
	bool onceReachNol() ;
	void reset() ;
};
