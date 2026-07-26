#include "SfmlGameEngine/Scene.h"
#include "SfmlGameEngine/Engine.h"
#include "SfmlGameEngine/SfmlTools.h"
#include "HelperCppClasses/StringTools.h"

namespace sfge {

void Scene::Render(sf::RenderTarget & rendertarget) {
}

void Scene::Update(float dt, const sf::Vector2i& mousePos, const std::vector<sf::Event>& events) {
}

void Scene::Init() {
}

void Scene::UnInit() {
}

void Scene::setEngine(Engine* engine)
{
	this->engine = engine;
}

Engine* Scene::getEngine() const
{
	return engine;
}

UniqueSprite Scene::loadSprite(const std::string& filename)
{
	textures.push_back(std::make_unique<sf::Texture>(getEngine()->getLanguages().getFileNameByLangIfExist(filename)));
	return std::make_unique<sf::Sprite>(*textures.back());
}

UniqueSound Scene::loadSound(const std::string& filename)
{
	sounds.push_back(std::make_unique<sf::SoundBuffer>(getEngine()->getLanguages().getFileNameByLangIfExist(filename)));
	return std::make_unique<sf::Sound>(*sounds.back());
}

std::unique_ptr<sf::SoundBuffer> Scene::loadSoundBuffer(const std::string& filename)
{
	return std::make_unique<sf::SoundBuffer>(getEngine()->getLanguages().getFileNameByLangIfExist(filename));
}

UniqueText Scene::loadText(const std::string& str, int size)
{
	return std::make_unique<sf::Text>(*engine->getDefaultFont(),SfmlTools::utf2text(str),size);
}

UniqueText Scene::loadText(int size)
{
	return loadText(std::string(), size);
}

UniqueText Scene::loadText(int size, sf::Color color)
{
	auto text = loadText(size);
	text->setFillColor(color);
	return text;
}

UniqueText Scene::loadText(const std::string& str, int size, sf::Color color)
{
	auto text = loadText(str, size);
	text->setFillColor(color);
	return text;
}

const Texts& Scene::getTexts() const
{
	return engine->getTexts();
}

const Colors& Scene::getColors() const
{
	return engine->getColors();
}

std::shared_ptr<Profile> Scene::getProfile() const
{
	return engine->getProfile();
}

void Scene::drawSpriteAt(sf::RenderTarget& rendertarget, sf::Sprite& sprite, float x, float y) {
	drawSpriteAt(rendertarget, sprite, { x,y });
}

void Scene::drawSpriteAt(sf::RenderTarget& rendertarget, sf::Sprite& sprite, sf::Vector2f pos) {
	sprite.setPosition(pos);
	rendertarget.draw(sprite);
}

void Scene::drawShapeAt(sf::RenderTarget& rendertarget, sf::Shape& shape, float x, float y) {
	drawShapeAt(rendertarget, shape, { x,y });
}

void Scene::drawShapeAt(sf::RenderTarget& rendertarget, sf::Shape& shape, sf::Vector2f pos) {
	shape.setPosition(pos);
	rendertarget.draw(shape);
}

void Scene::drawTextInBlockWidth(sf::RenderTarget& rendertarget,
	sf::Text& text, const std::string& str, float x, float y, float width, int redlinewidth)
{
	auto s1 = str;
	s1 = replaceAllString(s1, "\\n", "\\break ");
	s1 = replaceAllString(s1, "\r\n", "\\break ");
	s1 = replaceAllString(s1, "\r", "\\break ");
	s1 = replaceAllString(s1, "\n", "\\break ");
	auto words = splitString(s1, " ");
	if (words.size() == 0) return;

	auto line = std::string(redlinewidth, ' ') + words[0];
	for (int i = 1; i < words.size(); i++) {
		text.setString(SfmlTools::utf2text(line + "  " + words[i]));
		bool isnewline = line.ends_with("\\break");
		if ((text.getLocalBounds().size.x >= width) || isnewline) {
			line = replaceAllString(line, "\\break", "");
			text.setString(SfmlTools::utf2text(line));
			text.setPosition({ x, y });
			rendertarget.draw(text);
			y += text.getFont().getLineSpacing(text.getCharacterSize());
			line = words[i];
			if (isnewline) line = std::string(redlinewidth, ' ') + line;
		}
		else
			line += " " + words[i];
	}
	if (line.size() > 0) {
		line = replaceAllString(line, "\\break", "");
		text.setString(SfmlTools::utf2text(line));
		text.setPosition({ x, y });
		rendertarget.draw(text);
	}
}

}
