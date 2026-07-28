#pragma once

#include <SFML/System.hpp>

// Класс для расчета громкости по представлению и дистанции
class VolumeCalculator {
private:
	sf::Vector2f poslt ;
	sf::Vector2f posrb ;
	float dist ;
	float minvol ;
	float cx;
	float cy;
	float dx;
	float dy;
public:
	VolumeCalculator(sf::Vector2f poslt, sf::Vector2f posrb, float minvol);
	float getVolume(sf::Vector2f pos) const ;
} ;
