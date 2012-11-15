#pragma once

#include "stdafx.h"
#include "states/game_state.h"

namespace sun_magic {
	
	typedef sf::Event::KeyEvent KeyEvent;
	typedef sf::Event::TextEvent TextEvent;
	typedef sf::Event::MouseMoveEvent MouseMoveEvent;
	typedef sf::Event::MouseButtonEvent MouseButtonEvent;
	typedef sf::Event::MouseWheelEvent MouseWheelEvent;

	typedef sf::Mouse Mouse;
	typedef sf::Keyboard Keyboard;

	class GameObject;
	
	struct Event {
		enum EventType {
			// System
			E_CLOSED,
			E_GAME_STATE,

			// Mouse
			E_MOUSE_ENTERED,
			E_MOUSE_EXITED,
			E_MOUSE_MOVED,
			E_MOUSE_PRESSED,
			E_MOUSE_RELEASED,

			// Keyboard
			E_KEY_PRESSED,
			E_KEY_RELEASED,
			E_TEXT_ENTERED,

			// Trigger
			E_TRIGGER,

			// UI
			E_CLICKED,
		};

		EventType type;
		void *source;
		GameObject *focus;

		union
		{
			KeyEvent			key;			// E_KEY_PRESSED, E_KEY_RELEASED
			TextEvent           text;			// E_TEXT_ENTERED
			MouseMoveEvent      mouseMove;		// E_MOUSE_ENTERED, E_MOUSE_EXITED, E_MOUSE_MOVED
			MouseButtonEvent    mouseButton;	// E_MOUSE_PRESSED, E_MOUSE_RELEASED, E_CLICKED
		};
	};
	
	// Interface that all event listeners must implement
	class EventListener {
	public:
		virtual void ProcessEvent(Event event) = 0;
	};

}