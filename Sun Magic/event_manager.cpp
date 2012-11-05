#include "stdafx.h"
#include "event_manager.h"


namespace SunMagic {

	EventManager::EventManager(sf::RenderWindow *window)
		:_window(window)
	{}
	EventManager::~EventManager() {
		ClearCallbacks();
	}

	void EventManager::RegisterCallback(Event::EventType type, void (*callback)(Event*), GameObject *focus) {
		EventToFocusToCallbackSetMap::iterator event_iter = _event_focus_callback_map.find(type);
		FocusToCallbackSetMap *focus_callback_map;
		if (event_iter == _event_focus_callback_map.end()) {
			focus_callback_map = new FocusToCallbackSetMap();
			_event_focus_callback_map[type] = focus_callback_map;
		} else {
			focus_callback_map = event_iter->second;
		}
		
		FocusToCallbackSetMap::iterator focus_iter = focus_callback_map->find(focus);
		CallbackSet* callback_set;
		if (focus_iter == focus_callback_map->end()) {
			callback_set = new CallbackSet();
			focus_callback_map->insert(FocusToCallbackSetPair(focus, callback_set));
		} else {
			callback_set = focus_iter->second;
		}

		callback_set->insert(callback);
		_focus_objects.insert(focus);
	}

	bool EventManager::RemoveCallback(Event::EventType type, void (*callback)(Event*), GameObject *focus) {
		EventToFocusToCallbackSetMap::iterator event_iter = _event_focus_callback_map.find(type);
		if (event_iter == _event_focus_callback_map.end()) {
			return false;
		}
		FocusToCallbackSetMap *focus_callback_map = event_iter->second;
		
		FocusToCallbackSetMap::iterator focus_iter = focus_callback_map->find(focus);
		if (focus_iter == focus_callback_map->end()) {
			return false;
		}
		CallbackSet* callback_set = focus_iter->second;
		
		CallbackSet::iterator callback_iter = callback_set->find(callback);
		if (callback_iter == callback_set->end()) {
			return false;
		}

		if (callback_set->size() == 1) {
			std::hash_set<GameObject*>::iterator focus_object_iter = _focus_objects.find(focus);
			if (focus_object_iter != _focus_objects.end()) {
				_focus_objects.erase(focus_object_iter);
			}
		}
		callback_set->erase(callback_iter);
		return true;
	}

	void EventManager::ClearCallbacks() {
		for (EventToFocusToCallbackSetMap::iterator event_iter = _event_focus_callback_map.begin();
				event_iter != _event_focus_callback_map.end(); event_iter++) {
			FocusToCallbackSetMap *focus_callback_map = event_iter->second;
			for (FocusToCallbackSetMap::iterator focus_iter = focus_callback_map->begin();
					focus_iter != focus_callback_map->end(); focus_iter++) {
				CallbackSet* callback_set = focus_iter->second;
				delete callback_set;
			}
			delete focus_callback_map;
		}
		_event_focus_callback_map.clear();
	}

	void EventManager::AddEvent(Event event) {
		_events.push_back(event);
	}

	void EventManager::HandleInput() {
		sf::Event sf_event;
		Event event;
		MouseEvent mouse_event;
		KeyEvent key_event;
		while (_window->pollEvent(sf_event)) {
			switch (sf_event.type) {

			case sf::Event::Closed:
				event.type = Event::E_CLOSED;
				AddEvent(event);
				break;

			case sf::Event::MouseButtonPressed:
				mouse_event.type = Event::E_MOUSE_PRESSED;
				mouse_event.pos = sf::Vector2i(sf_event.mouseButton.x, sf_event.mouseButton.y);
				mouse_event.button = sf_event.mouseButton.button;
				AddEvent(mouse_event);
				break;
			case sf::Event::MouseButtonReleased:
				mouse_event.type = Event::E_MOUSE_RELEASED;
				mouse_event.pos = sf::Vector2i(sf_event.mouseButton.x, sf_event.mouseButton.y);
				mouse_event.button = sf_event.mouseButton.button;
				AddEvent(mouse_event);
				break;
			case sf::Event::MouseMoved:
				mouse_event.type = Event::E_MOUSE_MOVED;
				mouse_event.pos = sf::Vector2i(sf_event.mouseButton.x, sf_event.mouseButton.y);
				mouse_event.button = MouseButton::ButtonCount;
				AddEvent(mouse_event);

				// Update currently focused object
				UpdateFocus(mouse_event.pos);				
				break;

			case sf::Event::KeyPressed:
				key_event.type = Event::E_KEY_PRESSED;
				key_event.key = sf_event.key.code;
				AddEvent(key_event);
				break;
			case sf::Event::KeyReleased:
				key_event.type = Event::E_KEY_RELEASED;
				key_event.key = sf_event.key.code;
				AddEvent(key_event);
				break;
			}
		}
	}

	void EventManager::Update(float time) {
		for (std::vector<Event>::iterator iter = _events.begin(); iter != _events.end(); iter++) {
			Event event = *iter;

			EventToFocusToCallbackSetMap::iterator event_iter = _event_focus_callback_map.find(event.type);
			if (event_iter == _event_focus_callback_map.end()) {
				continue;
			}
			FocusToCallbackSetMap *focus_callback_map = event_iter->second;

			// Send event to all callbacks with NULL focus i.e. global focus
			FocusToCallbackSetMap::iterator focus_iter = focus_callback_map->find(NULL);
			if (focus_iter != focus_callback_map->end()) {
				CallbackSet* callback_set = focus_iter->second;
				for (CallbackSet::iterator callback_iter = callback_set->begin();
						callback_iter != callback_set->end(); callback_iter++) {
					void (*callback)(Event*) = *callback_iter;
					callback(&event);
				}
			}

			// Send event to all callbacks on the current focused object if any
			if (_focus != NULL) {
				focus_iter = focus_callback_map->find(_focus);
				if (focus_iter != focus_callback_map->end()) {
					CallbackSet* callback_set = focus_iter->second;
					for (CallbackSet::iterator callback_iter = callback_set->begin();
							callback_iter != callback_set->end(); callback_iter++) {
						void (*callback)(Event*) = *callback_iter;
						callback(&event);
					}
				}
			}
		}
	}

	int ZSort(GameObject* a, GameObject* b) {
		return a->GetZ() - b->GetZ();
	}

	void EventManager::UpdateFocus(sf::Vector2i mouse) {
		static sf::Vector2i old_mouse = sf::Mouse::getPosition();

		// Find all intersecting focus objects
		std::vector<GameObject*> intersections;
		for (std::hash_set<GameObject*>::iterator focus_iter = _focus_objects.begin(); focus_iter != _focus_objects.end(); focus_iter++) {
			GameObject *object = *focus_iter;
			if (object->GetRect().contains((float)mouse.x, (float)mouse.y)) {
				intersections.push_back(object);
			}
		}
		// Sort by z-order
		GameObject *new_focus = NULL;
		if (intersections.size() > 0) {
			std::sort(intersections.begin(), intersections.end(), ZSort);
			new_focus = intersections.front();
		}

		// If new focus send enter and exit events
		if (new_focus != _focus) {
			MouseEvent mouse_event;
			if (_focus != NULL) {
				// Exit event
				mouse_event.type = Event::E_MOUSE_EXITED;
				mouse_event.pos = old_mouse;
				EventToFocusToCallbackSetMap::iterator event_iter = _event_focus_callback_map.find(Event::E_MOUSE_EXITED);
				if (event_iter != _event_focus_callback_map.end()) {
					FocusToCallbackSetMap *focus_callback_map = event_iter->second;
					FocusToCallbackSetMap::iterator focus_iter = focus_callback_map->find(_focus);
					if (focus_iter != focus_callback_map->end()) {
						CallbackSet* callback_set = focus_iter->second;
						for (CallbackSet::iterator callback_iter = callback_set->begin(); callback_iter != callback_set->end(); callback_iter++) {
							void (*callback)(Event*) = *callback_iter;
							callback(&mouse_event);
						}
					}
				}
			}
			_focus = new_focus;
			if (_focus != NULL) {
				// Enter event
				mouse_event.type = Event::E_MOUSE_ENTERED;
				mouse_event.pos = mouse;
				EventToFocusToCallbackSetMap::iterator event_iter = _event_focus_callback_map.find(Event::E_MOUSE_ENTERED);
				if (event_iter != _event_focus_callback_map.end()) {
					FocusToCallbackSetMap *focus_callback_map = event_iter->second;
					FocusToCallbackSetMap::iterator focus_iter = focus_callback_map->find(_focus);
					if (focus_iter != focus_callback_map->end()) {
						CallbackSet* callback_set = focus_iter->second;
						for (CallbackSet::iterator callback_iter = callback_set->begin(); callback_iter != callback_set->end(); callback_iter++) {
							void (*callback)(Event*) = *callback_iter;
							callback(&mouse_event);
						}
					}
				}
			}
		}
		old_mouse = mouse;
	}
}