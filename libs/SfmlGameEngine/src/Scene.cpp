#include "SfmlGameEngine/Scene.h"

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

std::unique_ptr<sf::Sprite> Scene::loadSprite(const std::string& filename)
{
	textures.push_back(std::make_unique<sf::Texture>(filename));
	return std::make_unique<sf::Sprite>(*textures.back());
}

}
