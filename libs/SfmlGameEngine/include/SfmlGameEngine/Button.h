#pragma once

#include <SFML/Graphics.hpp>

namespace sfge {

class Button: public sf::Drawable
{
private:
	sf::Text caption;
	sf::RectangleShape shape;
	std::function<void()> clickfunc;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
public:
	Button(const sf::Font& font, const sf::String & text, int size, int x, int y, int w, int h);
	bool isMouseOver(sf::Vector2i mousePos) const;
	void setOnClick(std::function<void()> clickfunc);
	void processEvent(const sf::Event& event);
};

}