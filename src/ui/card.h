#pragma once

#include <stdafx.h>
#include "animations/move_animation.h"
#include "ui/ui_element.h"

namespace sun_magic {

	class Card : public UiElement {
	public:
		Card(float hide_x, float hide_y, float show_x, float show_y, float width, float height);
		~Card();

		void Hide();
		void Show();
		void Update(float elapsed_time);

	private:
		sf::Vector2f hide_;
		sf::Vector2f show_;
		MoveAnimation animation_;
	};

}