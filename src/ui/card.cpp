#include <stdafx.h>
#include "animations/move_animation.h"
#include "ui/card.h"

namespace sun_magic {

	Card::Card(float hide_x, float hide_y, float show_x, float show_y, float width, float height) :
		UiElement(hide_x, hide_y, width, height),
		hide_(sf::Vector2f(hide_x, hide_y)),
		show_(sf::Vector2f(show_x, show_y)),
		animation_(this, 1.0f)
	{
	}
	Card::~Card() {}

	void Card::Hide() {
		animation_.SetEnd(hide_);
	}
	void Card::Show() {
		animation_.SetEnd(show_);
	}

	void Card::Update(float elapsed_time) {
		animation_.Update(elapsed_time);
	}

}