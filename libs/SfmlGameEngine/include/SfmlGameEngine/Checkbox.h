#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

namespace sfge {

class Checkbox: public sf::Drawable
{
private:
	sf::Text caption;
	sf::RectangleShape shape;
	sf::VertexArray vertex;
	bool checked = false;
	bool disabled = false;
public:
	Checkbox(const sf::Font& font, const sf::String & text, int size, int x, int y, int w, int h);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	bool isMouseOver(sf::Vector2i mousePos) const;
	bool isChecked() const;
	void setChecked(bool value);
	void setDisabled(bool value);
	void processEvent(const sf::Event& event);
};

using UniqueCheckbox = std::unique_ptr<Checkbox>;

}