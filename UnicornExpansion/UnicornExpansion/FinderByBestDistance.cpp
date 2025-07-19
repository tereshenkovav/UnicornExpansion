#include "FinderByBestDistance.h"

FinderByBestDistance::FinderByBestDistance(int min_dist, const sf::Vector2f& start)
{
	this->min_dist = min_dist;
	this->start = start;
	reset();
}

void FinderByBestDistance::reset() {
	best_dist = 0.0f;
	best_idx = -1;
}

void FinderByBestDistance::addPos(const sf::Vector2f& pos, int idx) {
	sf::Vector2f dist = start - pos;
	if (dist.length() <= min_dist)
		if ((best_idx == -1) || (dist.length() < best_dist)) {
			best_idx = idx; 
			best_dist = dist.length();
		}
}

std::optional<int> FinderByBestDistance::getBestIndex() const {
	if (best_idx == -1) return std::nullopt; else return std::optional<int>(best_idx);
}