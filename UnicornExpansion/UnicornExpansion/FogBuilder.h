#pragma once

#include <vector>
#include <map>
#include <optional>
#include "Game.h"
#include <SFML/Graphics.hpp>

enum class FogType { Left, Right, Top, Bottom, LeftTop, RightTop, LeftBottom, RightBottom,
  TailRightBottom, TailLeftBottom, TailRightTop, TailLeftTop };

class FogBuilder
{
private:
	int width;
	int height;
	sf::Texture tex;
	std::map<FogType, std::unique_ptr<sf::Sprite>> fogs;
	std::vector<std::unique_ptr<sf::Texture>> texs;
	std::vector<std::vector<std::optional<FogType>>> map;
public:
	FogBuilder();
	void initByGame(const Game& game);
	void updateByGame(const Game& game);
	std::optional<sf::Sprite> getFogSprite(int x, int y) const;

};

