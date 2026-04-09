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

}