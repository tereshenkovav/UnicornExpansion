#include "SfmlGameEngine/CheckboxGroup.h"
#include "SfmlGameEngine/SfmlTools.h"

namespace sfge {
	void CheckboxGroup::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		for (auto & cb : checkboxes)
			cb->draw(target, states);
	}

	void CheckboxGroup::addCheckbox(const sf::Font& font, const sf::String& text, int size, int x, int y, int w, int h)
	{
		checkboxes.push_back(std::make_unique<Checkbox>(font, text, size, x, y, w, h));
	}

	int CheckboxGroup::getCheckedIdx() const {
		for (int i = 0; i < checkboxes.size(); i++)
			if (checkboxes[i]->isChecked()) return i;
		return -1;
	}

	void CheckboxGroup::setCheckedIdx(int idx) {
		for (int i = 0; i < checkboxes.size(); i++)
			checkboxes[i]->setChecked(i == idx);
	}

	void CheckboxGroup::processEvent(const sf::Event& event) {
		for (int i = 0; i < checkboxes.size(); i++) {
			bool v = checkboxes[i]->isChecked();
			checkboxes[i]->processEvent(event);
			if ((!v) && checkboxes[i]->isChecked()) {
				for (int j = 0; j < checkboxes.size(); j++)
					if (i != j) checkboxes[j]->setChecked(false);
			}
			else {
				if (v && (!checkboxes[i]->isChecked())) checkboxes[i]->setChecked(true);
			}
		}
	}
}
