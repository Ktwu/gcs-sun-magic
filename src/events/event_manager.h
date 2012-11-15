#pragma once

#include "stdafx.h"
#include "events/event.h"
	
namespace sun_magic {

	typedef std::hash_set<EventListener*> ListenerSet;
	typedef std::hash_map<GameObject*, ListenerSet*> FocusToListenerSetMap;
	typedef std::pair<GameObject*, ListenerSet*> FocusToListenerSetPair;
	typedef std::hash_map<Event::EventType, FocusToListenerSetMap*> EventToFocusToListenerSetMap;

	class GameObject;

	class EventManager {
	public:
		EventManager();
		~EventManager();

		// Only game objects that have been added, will be considered candidates for recieving focus.
		void AddGameObject(GameObject* object);

		bool RemoveGameObject(GameObject* object);

		void ClearGameObjects();

		std::vector<GameObject*>& GetGameObjects();

		// Gets the current focus. This is the GameObject that the mouse is intersecting with the lowest Z value.
		GameObject* GetFocus();

		/* Registers a listener for an event type.
			* If focus is NULL then the listener will be called anytime the event is triggered.
			* Otherwise, the listener will only be called when the focus is the current focus.
			*/
		void RegisterListener(Event::EventType type, EventListener *listener, GameObject *focus = NULL);

		bool UnregisterListener(Event::EventType type, EventListener *listener, GameObject *focus = NULL);

		void ClearListeners();

		// Use this to add custom events
		void AddEvent(Event *event);

		// This is used to add system events
		void AddEvent(sf::Event event);

		void Update(); // Propagate events to listeners

		void UpdateObjects(float elapsed_time);

		void DrawObjects(sf::RenderTarget *target);

	protected:
		void UpdateFocus(sf::Vector2i mouse);

		// Map of listeners registered for each focus object for each event type
		EventToFocusToListenerSetMap eventfocus__listener_map_;
		std::vector<Event*> events_;
		std::vector<GameObject*> game_objects_;	// Set of all objects that can be focused
		GameObject *focus_;						// Currently focused object
	};

}