#pragma once

#include "stdafx.h"
#include "game_object.h"
#include "game_state.h"

namespace sun_magic {
	
	typedef sf::Mouse Mouse;
	typedef sf::Keyboard Keyboard;
	
	struct Event {
		enum EventType {
			// System
			E_CLOSED,
			E_GAME_STATE,

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
		void *source;
		GameObject *focus;
	};

	struct GameStateEvent : Event {
		ref::GameState state;
	};

	struct MouseEvent : Event {
		sf::Vector2i pos;
		Mouse::Button button;
	};

	struct KeyEvent : Event {
		Keyboard::Key key;
	};

	
	// Interface that all event listeners must implement
	class EventListener {
	public:
		virtual void ProcessEvent(Event *event) = 0;
	};
}