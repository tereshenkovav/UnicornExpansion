#include "SfmlGameEngine/Checkbox.h"
#include "SfmlGameEngine/SfmlTools.h"

const sf::Color COLNORM = sf::Color({ 40, 40, 40, 192 });
const sf::Color COLOVER = sf::Color({ 80, 80, 80, 192 });
const sf::Color COLBORDERNORM = sf::Color({ 208, 0, 220 });
const sf::Color COLBORDEROVER = sf::Color({ 228, 0, 255 });
const sf::Color COLBORDERDISABLED = sf::Color({ 128, 128, 128 });
const sf::Color COLFONTNORM = sf::Color({ 192, 192, 192 });
const sf::Color COLFONTOVER = sf::Color::White;
const sf::Color COLFONTDISABLED = sf::Color({ 128, 128, 128 });
const sf::Vector2f SHIFT = sf::Vector2f({ -3.0f,-3.0f });

namespace sfge {
	void Checkbox::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		target.draw(shape, states);
		target.draw(caption, states);
		if (checked) target.draw(vertex, states);
	}

	Checkbox::Checkbox(const sf::Font& font, const sf::String& text, int size, int x, int y, int w, int h):
		caption(font, text, size)
	{
		shape.setPosition({ (float)x,(float)y });
		shape.setSize({ (float)w,(float)h });
		shape.setOutlineThickness(1);
		shape.setOutlineColor(COLBORDERNORM);
		shape.setFillColor(COLNORM);
				
		vertex = sf::VertexArray(sf::PrimitiveType::Triangles, 6);
		vertex[0].position = sf::Vector2f(x + 2, y + h / 2);
		vertex[1].position = sf::Vector2f(x + w / 2, y + h / 2 + 4);
		vertex[2].position = sf::Vector2f(x + w / 2, y + h - 2);
		vertex[3].position = sf::Vector2f(x + w - 2, y + 2);
		vertex[4].position = sf::Vector2f(x + w / 2, y + h / 2 + 4);
		vertex[5].position = sf::Vector2f(x + w / 2, y + h - 2);
		
		caption.setFillColor(COLFONTNORM);
		caption.setPosition({ x + w + 10.0f, (float)y });
	}

	bool Checkbox::isMouseOver(sf::Vector2i mousePos) const {
		return shape.getGlobalBounds().contains({ (float)mousePos.x, (float)mousePos.y }) ||
			caption.getGlobalBounds().contains({ (float)mousePos.x, (float)mousePos.y });
	}

	bool Checkbox::isChecked() const {
		return checked;
	}

	void Checkbox::setChecked(bool value) {
		checked = value;
	}

	void Checkbox::setDisabled(bool value)
	{
		disabled = value;
		shape.setOutlineColor(disabled ? COLBORDERDISABLED : COLBORDERNORM);
		caption.setFillColor(disabled ? COLFONTDISABLED : COLFONTNORM);
		for (int i = 0; i < vertex.getVertexCount(); i++)
			vertex[i].color = disabled ? COLFONTDISABLED : sf::Color::White;
	}

	void Checkbox::processEvent(const sf::Event& event) {
		if (disabled) return;

		if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonReleased>()) {
			if (isMouseOver(mousePressed->position)) setChecked(!checked);
		}
		if (const auto* mouseMoved = event.getIf<sf::Event::MouseMoved>()) {
			shape.setFillColor(isMouseOver(mouseMoved->position) ? COLOVER : COLNORM);
			shape.setOutlineColor(isMouseOver(mouseMoved->position) ? COLBORDEROVER : COLBORDERNORM);
			caption.setFillColor(isMouseOver(mouseMoved->position) ? COLFONTOVER : COLFONTNORM);
		}
	}
}
