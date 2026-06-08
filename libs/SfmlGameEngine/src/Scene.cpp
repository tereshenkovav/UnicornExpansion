#include "SfmlGameEngine/Scene.h"
#include "SfmlGameEngine/Engine.h"
#include "SfmlGameEngine/SfmlTools.h"

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

}
