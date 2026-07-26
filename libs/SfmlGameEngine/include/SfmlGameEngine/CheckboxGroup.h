#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SfmlGameEngine/Checkbox.h>

namespace sfge {

class CheckboxGroup: public sf::Drawable
{
private:
	std::vector<std::unique_ptr<Checkbox>> checkboxes ;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
public:
	void addCheckbox(const sf::Font& font, const sf::String & text, int size, int x, int y, int w, int h) ;
	int getCheckedIdx() const;
	void setCheckedIdx(int idx);
	void processEvent(const sf::Event& event);
};

using UniqueCheckboxGroup = std::unique_ptr<CheckboxGroup>;

}