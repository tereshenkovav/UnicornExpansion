#include "FogBuilder.h"
#include "GameUnit.h"

FogBuilder::FogBuilder()
{
	sf::Image img;

	img.resize({ BLOCKW, BLOCKH }, sf::Color{ 0,0,0,0 });
	for (unsigned int i = 0; i < BLOCKW; i++)
		for (unsigned int j = 0; j < BLOCKH / 4; j++)
			if ((i / 2 + j / 2) % 2 == 0) img.setPixel({ i, j }, sf::Color{ 0,0,0,255 });
	texs.push_back(std::make_unique<sf::Texture>(img));
	fogs[FogType::Bottom] = std::make_unique<sf::Sprite>(*texs.back());

	img.resize({ BLOCKW, BLOCKH }, sf::Color{ 0,0,0,0 });
	for (unsigned int i = 0; i < BLOCKW; i++)
		for (unsigned int j = 3 * BLOCKH / 4; j < BLOCKH; j++)
			if ((i / 2 + j / 2) % 2 == 0) img.setPixel({ i, j }, sf::Color{ 0,0,0,255 });
	texs.push_back(std::make_unique<sf::Texture>(img));
	fogs[FogType::Top] = std::make_unique<sf::Sprite>(*texs.back());

	img.resize({ BLOCKW, BLOCKH }, sf::Color{ 0,0,0,0 });
	for (unsigned int i = 0; i < BLOCKW / 4; i++)
		for (unsigned int j = 0; j < BLOCKH; j++)
			if ((i / 2 + j / 2) % 2 == 0) img.setPixel({ i, j }, sf::Color{ 0,0,0,255 });
	texs.push_back(std::make_unique<sf::Texture>(img));
	fogs[FogType::Right] = std::make_unique<sf::Sprite>(*texs.back());

	img.resize({ BLOCKW, BLOCKH }, sf::Color{ 0,0,0,0 });
	for (unsigned int i = 3 * BLOCKW / 4; i < BLOCKW; i++)
		for (unsigned int j = 0; j < BLOCKH; j++)
			if ((i / 2 + j / 2) % 2 == 0) img.setPixel({ i, j }, sf::Color{ 0,0,0,255 });
	texs.push_back(std::make_unique<sf::Texture>(img));
	fogs[FogType::Left] = std::make_unique<sf::Sprite>(*texs.back());

	// Углы тумана
	img.resize({ BLOCKW, BLOCKH }, sf::Color{ 0,0,0,0 });
	for (unsigned int i = 0; i < BLOCKW; i++)
		for (unsigned int j = 0; j < BLOCKH; j++) {
			int r = (BLOCKW - i) * (BLOCKW - i) + (BLOCKH - j) * (BLOCKH - j);
			if (r > BLOCKW * BLOCKH)
				img.setPixel({ i, j }, sf::Color{ 0,0,0,255 });
			else
				if ((r > (3 * BLOCKW / 4) * (3 * BLOCKH / 4)) && ((i / 2 + j / 2) % 2 == 0))
					img.setPixel({ i, j }, sf::Color{ 0,0,0,255 });
		}
	texs.push_back(std::make_unique<sf::Texture>(img));
	fogs[FogType::LeftBottom] = std::make_unique<sf::Sprite>(*texs.back());

	img.resize({ BLOCKW, BLOCKH }, sf::Color{ 0,0,0,0 });
	for (unsigned int i = 0; i < BLOCKW; i++)
		for (unsigned int j = 0; j < BLOCKH; j++) {
			int r = (i * i) + (BLOCKH - j) * (BLOCKH - j);
			if (r > BLOCKW * BLOCKH)
				img.setPixel({ i, j }, sf::Color{ 0,0,0,255 });
			else
				if ((r > (3 * BLOCKW / 4) * (3 * BLOCKH / 4)) && ((i / 2 + j / 2) % 2 == 0))
					img.setPixel({ i, j }, sf::Color{ 0,0,0,255 });
		}
	texs.push_back(std::make_unique<sf::Texture>(img));
	fogs[FogType::RightBottom] = std::make_unique<sf::Sprite>(*texs.back());

	img.resize({ BLOCKW, BLOCKH }, sf::Color{ 0,0,0,0 });
	for (unsigned int i = 0; i < BLOCKW; i++)
		for (unsigned int j = 0; j < BLOCKH; j++) {
			int r = (BLOCKW - i) * (BLOCKW - i) + (j * j);
			if (r > BLOCKW * BLOCKH)
				img.setPixel({ i, j }, sf::Color{ 0,0,0,255 });
			else
				if ((r > (3 * BLOCKW / 4) * (3 * BLOCKH / 4)) && ((i / 2 + j / 2) % 2 == 0))
					img.setPixel({ i, j }, sf::Color{ 0,0,0,255 });
		}
	texs.push_back(std::make_unique<sf::Texture>(img));
	fogs[FogType::LeftTop] = std::make_unique<sf::Sprite>(*texs.back());

	img.resize({ BLOCKW, BLOCKH }, sf::Color{ 0,0,0,0 });
	for (unsigned int i = 0; i < BLOCKW; i++)
		for (unsigned int j = 0; j < BLOCKH; j++) {
			int r = (i * i) + (j * j);
			if (r > BLOCKW * BLOCKH)
				img.setPixel({ i, j }, sf::Color{ 0,0,0,255 });
			else
				if ((r > (3 * BLOCKW / 4) * (3 * BLOCKH / 4)) && ((i / 2 + j / 2) % 2 == 0))
					img.setPixel({ i, j }, sf::Color{ 0,0,0,255 });
		}
	texs.push_back(std::make_unique<sf::Texture>(img));
	fogs[FogType::RightTop] = std::make_unique<sf::Sprite>(*texs.back());

	// Маленькие хвостики сетчатого тумана для уголков
	img.resize({ BLOCKW, BLOCKH }, sf::Color{ 0,0,0,0 });
	for (unsigned int i = 0; i < BLOCKW; i++)
		for (unsigned int j = 0; j < BLOCKH; j++) {
			int r = (i * i) + (j * j);
			if ((r < BLOCKW * BLOCKH / 16) && ((i / 2 + j / 2) % 2 == 0))
				img.setPixel({ i, j }, sf::Color{ 0,0,0,255 });
		}
	texs.push_back(std::make_unique<sf::Texture>(img));
	fogs[FogType::TailRightBottom] = std::make_unique<sf::Sprite>(*texs.back());

	img.resize({ BLOCKW, BLOCKH }, sf::Color{ 0,0,0,0 });
	for (unsigned int i = 0; i < BLOCKW; i++)
		for (unsigned int j = 0; j < BLOCKH; j++) {
			int r = (BLOCKW - i) * (BLOCKW - i) + (j * j);
			if ((r < BLOCKW * BLOCKH / 16) && ((i / 2 + j / 2) % 2 == 0))
				img.setPixel({ i, j }, sf::Color{ 0,0,0,255 });
		}
	texs.push_back(std::make_unique<sf::Texture>(img));
	fogs[FogType::TailLeftBottom] = std::make_unique<sf::Sprite>(*texs.back());

	img.resize({ BLOCKW, BLOCKH }, sf::Color{ 0,0,0,0 });
	for (unsigned int i = 0; i < BLOCKW; i++)
		for (unsigned int j = 0; j < BLOCKH; j++) {
			int r = (i * i) + (BLOCKH - j) * (BLOCKH - j);
			if ((r < BLOCKW * BLOCKH / 16) && ((i / 2 + j / 2) % 2 == 0))
				img.setPixel({ i, j }, sf::Color{ 0,0,0,255 });
		}
	texs.push_back(std::make_unique<sf::Texture>(img));
	fogs[FogType::TailRightTop] = std::make_unique<sf::Sprite>(*texs.back());

	img.resize({ BLOCKW, BLOCKH }, sf::Color{ 0,0,0,0 });
	for (unsigned int i = 0; i < BLOCKW; i++)
		for (unsigned int j = 0; j < BLOCKH; j++) {
			int r = (BLOCKW - i) * (BLOCKW - i) + (BLOCKH - j) * (BLOCKH - j);
			if ((r < BLOCKW * BLOCKH / 16) && ((i / 2 + j / 2) % 2 == 0))
				img.setPixel({ i, j }, sf::Color{ 0,0,0,255 });
		}
	texs.push_back(std::make_unique<sf::Texture>(img));
	fogs[FogType::TailLeftTop] = std::make_unique<sf::Sprite>(*texs.back());
}

void FogBuilder::initByGame(const Game& game)
{
	this->width = game.getWidth();
	this->height = game.getHeight();
	map.resize(width);
	for (int i = 0; i < width; i++)
		map[i].resize(height);
	updateByGame(game);
}

void FogBuilder::updateByGame(const Game& game)
{
	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++) {
			map[i][j] = std::nullopt;
			if (!game.isFog(i, j)) {
				bool ftop = game.isFog(i, j - 1);
				bool fbottom = game.isFog(i, j + 1);
				bool fleft = game.isFog(i - 1, j);
				bool fright = game.isFog(i + 1, j);
				if ((j > 0) && ftop && (!fleft) && (!fright)) map[i][j] = FogType::Bottom;
				if ((j < height - 1) && fbottom && (!fleft) && (!fright)) map[i][j] = FogType::Top;
				if ((i > 0) && fleft && (!ftop) && (!fbottom)) map[i][j] = FogType::Right;
				if ((i < width - 1) && fright && (!ftop) && (!fbottom)) map[i][j] = FogType::Left;
				if (((i > 0) || (j > 0)) && (ftop && fleft && (!fright))) map[i][j] = FogType::LeftBottom;
				if (((i < width - 1) || (j > 0)) && (ftop && (!fleft) && (fright))) map[i][j] = FogType::RightBottom;
				if (((i > 0) || (j < height - 1)) && (fbottom && fleft && (!fright))) map[i][j] = FogType::LeftTop;
				if (((i < width - 1) || (j < height - 1)) && (fbottom && (!fleft) && (fright))) map[i][j] = FogType::RightTop;
				if ((!ftop) && (!fbottom) && (!fleft) && (!fright)) {					
					if (game.isFog(i - 1, j - 1)) map[i][j] = FogType::TailRightBottom;
					if (game.isFog(i + 1, j - 1)) map[i][j] = FogType::TailLeftBottom;
					if (game.isFog(i - 1, j + 1)) map[i][j] = FogType::TailRightTop;
					if (game.isFog(i + 1, j + 1)) map[i][j] = FogType::TailLeftTop;
				}
			}				
		}
}

std::optional<sf::Sprite> FogBuilder::getFogSprite(int x, int y) const
{
	if (map[x][y]) return *fogs.at(*(map[x][y])); else return std::nullopt;
}
