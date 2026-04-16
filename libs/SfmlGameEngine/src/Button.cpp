#include "SfmlGameEngine/Button.h"
#include "SfmlGameEngine/SfmlTools.h"

const sf::Color COLNORM = sf::Color({ 40, 40, 40, 192 });
const sf::Color COLOVER = sf::Color({ 80, 80, 80, 192 });
const sf::Color COLBORDERNORM = sf::Color({ 208, 0, 220 });
const sf::Color COLBORDEROVER = sf::Color({ 228, 0, 255 });
const sf::Color COLFONTNORM = sf::Color({ 192, 192, 192 });
const sf::Color COLFONTOVER = sf::Color::White;
const sf::Vector2f SHIFT = sf::Vector2f({ -3.0f,-3.0f });

namespace sfge {
	void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		target.draw(shape, states);
		target.draw(caption, states);
	}

	Button::Button(const sf::Font& font, const sf::String& text, int size, int x, int y, int w, int h):
		caption(font, text, size)
	{
		shape.setPosition({ (float)x,(float)y });
		shape.setSize({ (float)w,(float)h });
		shape.setOutlineThickness(1);
		shape.setOutlineColor(COLBORDERNORM);
		shape.setFillColor(COLNORM);

		caption.setFillColor(COLFONTNORM);
		caption.setPosition({ x + w/2 - caption.getLocalBounds().size.x / 2, y + 8.0f});
	}

	bool Button::isMouseOver(sf::Vector2i mousePos) const {
		return shape.getGlobalBounds().contains({ (float)mousePos.x, (float)mousePos.y });
	}

	void Button::setOnClick(std::function<void()> clickfunc) {
		this->clickfunc = clickfunc;
	}

	void Button::processEvent(const sf::Event& event) {
		if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonReleased>()) {
			shape.setOrigin({ 0.0f, 0.0f });
			caption.setOrigin({ 0.0f, 0.0f });
			if (isMouseOver(mousePressed->position)) clickfunc();
		}
		if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>()) {
			if (isMouseOver(mousePressed->position)) {
				shape.setOrigin(SHIFT);
				caption.setOrigin(SHIFT);
			}
		}
		if (const auto* mouseMoved = event.getIf<sf::Event::MouseMoved>()) {
			shape.setFillColor(isMouseOver(mouseMoved->position) ? COLOVER : COLNORM);
			shape.setOutlineColor(isMouseOver(mouseMoved->position) ? COLBORDEROVER : COLBORDERNORM);
			caption.setFillColor(isMouseOver(mouseMoved->position) ? COLFONTOVER : COLFONTNORM);
		}
	}
}
