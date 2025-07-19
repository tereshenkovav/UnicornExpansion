#pragma once

#include <SFML/System.hpp>

// Служебный класс для поиска лучшей дистанции из переданных
class FinderByBestDistance
{
private:
	sf::Vector2f start;
	int min_dist;
	float best_dist;
	int best_idx;
public:
	// Создаем с указанием максимально обрабатываемой дистанции и точки начала
	FinderByBestDistance(int min_dist, const sf::Vector2f & start);
	void reset();
	// Добавляем позицию с индексом
	void addPos(const sf::Vector2f& pos, int idx);
	// Получить индекс лучшей позиции, если есть
	std::optional<int> getBestIndex() const;
};

