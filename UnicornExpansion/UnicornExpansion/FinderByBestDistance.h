#pragma once

#include <SFML/System.hpp>

// ��������� ����� ��� ������ ������ ��������� �� ����������
class FinderByBestDistance
{
private:
	sf::Vector2f start;
	int min_dist;
	float best_dist;
	int best_idx;
public:
	// ������� � ��������� ����������� �������������� ��������� � ����� ������
	FinderByBestDistance(int min_dist, const sf::Vector2f & start);
	void reset();
	// ��������� ������� � ��������
	void addPos(const sf::Vector2f& pos, int idx);
	// �������� ������ ������ �������, ���� ����
	std::optional<int> getBestIndex() const;
};

