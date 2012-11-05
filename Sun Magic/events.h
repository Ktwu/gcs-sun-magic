#pragma once

#include "stdafx.h"
#include "game_object.h"

namespace SunMagic {
	
	typedef sf::Mouse::Button MouseButton;
	typedef sf::Keyboard::Key Key;
	
	struct Event {
		enum EventType {
			// System
			E_CLOSED,

			// Mouse
			E_MOUSE_ENTERED,
			E_MOUSE_EXITED,
			E_MOUSE_PRESSED,
			E_MOUSE_RELEASED,
			E_MOUSE_MOVED,

			// Keyboard
			E_KEY_PRESSED,
			E_KEY_RELEASED,

			// Trigger
			E_TRIGGER,
		};

		EventType type;
	};

	struct MouseEvent : Event {
		sf::Vector2i pos;
		MouseButton button;
	};

	struct KeyEvent : Event {
		Key key;
	};

	struct TriggerEvent : Event {
		GameObject *source;
	};

}