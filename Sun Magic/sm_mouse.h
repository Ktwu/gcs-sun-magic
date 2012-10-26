#pragma once

#include "stdafx.h"

namespace SunMagic {

	class SMMouse {
	public:
		bool haveButtonPressEvent;
		bool haveButtonReleaseEvent;
		bool haveMoveEvent;
		bool haveWheelEvent;

		sf::Event::MouseButtonEvent buttonPressEvent;
		sf::Event::MouseButtonEvent buttonReleaseEvent;
		sf::Event::MouseMoveEvent moveEvent;
		sf::Event::MouseWheelEvent wheelEvent;

		void reset() {
			haveButtonPressEvent = false;
			haveButtonReleaseEvent = false;
			haveMoveEvent = false;
			haveWheelEvent = false;
		}
	};

}