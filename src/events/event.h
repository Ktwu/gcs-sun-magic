#pragma once

#include "stdafx.h"
#include "events/gameevent.h"
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
			E_LOAD_STATE,
			E_GAME_STATE,

			// Game
			E_GAME_EVENT,

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

			// Hiragana
			E_HIRAGANA_DRAWN
		};

		EventType type;
		void *source;
		GameObject *focus;
		sf::String message;		// E_HIRAGANA_DRAWN, also C++ is stupid

		sf::Vector2i relativeMousePosition;

		union
		{
			KeyEvent			key;			// E_KEY_PRESSED, E_KEY_RELEASED
			TextEvent           text;			// E_TEXT_ENTERED
			MouseMoveEvent      mouseMove;		// E_MOUSE_ENTERED, E_MOUSE_EXITED, E_MOUSE_MOVED
			MouseButtonEvent    mouseButton;	// E_MOUSE_PRESSED, E_MOUSE_RELEASED, E_CLICKED
			GameEvent			gameEvent;		// E_GAME_EVENT
		};
	};
	
	// Interface that all event listeners must implement
	class EventListener {
	public:
		virtual void ProcessEvent(Event event) = 0;
	};

}