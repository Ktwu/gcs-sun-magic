#pragma once

#include "stdafx.h"
#include "game_object.h"

namespace sun_magic {
	
	typedef sf::Mouse Mouse;
	typedef sf::Keyboard Keyboard;
	
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
		Mouse::Button button;
	};

	struct KeyEvent : Event {
		Keyboard::Key key;
	};

	struct TriggerEvent : Event {
		GameObject *source;
	};

	
	// Interface that all event listeners must implement
	class EventListener {
	public:
		virtual void ProcessEvent(Event *event) = 0;
	};
}