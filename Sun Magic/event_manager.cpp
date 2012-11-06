#include "stdafx.h"
#include "event_manager.h"

namespace sun_magic {

	EventManager::EventManager() {}
	EventManager::~EventManager() {
		ClearListeners();
	}

	GameObject* EventManager::GetFocus() {
		return focus_;
	}

	void EventManager::AddGameObject(GameObject *object) {
		game_objects_.push_back(object);
		object->Register();
	}
	
	bool EventManager::RemoveGameObject(GameObject *object) {
		for (std::vector<GameObject*>::iterator focus_iter = game_objects_.begin(); focus_iter != game_objects_.end(); focus_iter++) {
			if (*focus_iter == object) {
				game_objects_.erase(focus_iter);
				object->Unregister();
				if (object == focus_) {
					focus_ = NULL;
				}
				return true;
			}
		}
		return false;
	}

	std::vector<GameObject*>& EventManager::GetGameObjects() {
		return game_objects_;
	}

	void EventManager::RegisterListener(Event::EventType type, EventListener *listener, GameObject *focus) {
		EventToFocusToListenerSetMap::iterator event_iter = eventfocus__listener_map_.find(type);
		FocusToListenerSetMap *focus_listener_map;
		if (event_iter == eventfocus__listener_map_.end()) {
			focus_listener_map = new FocusToListenerSetMap();
			eventfocus__listener_map_[type] = focus_listener_map;
		} else {
			focus_listener_map = event_iter->second;
		}
		
		FocusToListenerSetMap::iterator focus_iter = focus_listener_map->find(focus);
		ListenerSet* listener_set;
		if (focus_iter == focus_listener_map->end()) {
			listener_set = new ListenerSet();
			focus_listener_map->insert(FocusToListenerSetPair(focus, listener_set));
		} else {
			listener_set = focus_iter->second;
		}

		listener_set->insert(listener);
	}

	bool EventManager::UnregisterListener(Event::EventType type, EventListener *listener, GameObject *focus) {
		EventToFocusToListenerSetMap::iterator event_iter = eventfocus__listener_map_.find(type);
		if (event_iter == eventfocus__listener_map_.end()) {
			return false;
		}
		FocusToListenerSetMap *focus_listener_map = event_iter->second;
		
		FocusToListenerSetMap::iterator focus_iter = focus_listener_map->find(focus);
		if (focus_iter == focus_listener_map->end()) {
			return false;
		}
		ListenerSet* listener_set = focus_iter->second;
		
		ListenerSet::iterator listener_iter = listener_set->find(listener);
		if (listener_iter == listener_set->end()) {
			return false;
		}

		listener_set->erase(listener_iter);
		return true;
	}

	void EventManager::ClearListeners() {
		for (EventToFocusToListenerSetMap::iterator event_iter = eventfocus__listener_map_.begin();
				event_iter != eventfocus__listener_map_.end(); event_iter++) {
			FocusToListenerSetMap *focus_listener_map = event_iter->second;
			for (FocusToListenerSetMap::iterator focus_iter = focus_listener_map->begin();
					focus_iter != focus_listener_map->end(); focus_iter++) {
				ListenerSet* listener_set = focus_iter->second;
				delete listener_set;
			}
			delete focus_listener_map;
		}
		eventfocus__listener_map_.clear();
	}

	void EventManager::AddEvent(Event *event) {
		events_.push_back(event);
	}

	void EventManager::AddEvent(sf::Event sf_event) {
		switch (sf_event.type) {

		case sf::Event::Closed:
			{
				Event *event = new Event();
				event->type = Event::E_CLOSED;
				AddEvent(event);
				break;
			}

		case sf::Event::MouseButtonPressed:
		case sf::Event::MouseButtonReleased:
			{
				MouseEvent *mouse_event = new MouseEvent();
				mouse_event->pos = sf::Vector2i(sf_event.mouseButton.x, sf_event.mouseButton.y);
				mouse_event->button = sf_event.mouseButton.button;
				switch (sf_event.type) {
				case sf::Event::MouseButtonPressed:
					mouse_event->type = Event::E_MOUSE_PRESSED;
					break;
				case sf::Event::MouseButtonReleased:
					mouse_event->type = Event::E_MOUSE_RELEASED;
					break;
				}
				AddEvent(mouse_event);
				break;
			}
		case sf::Event::MouseMoved:
			{
				MouseEvent *mouse_event = new MouseEvent();
				mouse_event->pos = sf::Vector2i(sf_event.mouseMove.x, sf_event.mouseMove.y);
				mouse_event->type = Event::E_MOUSE_MOVED;
				AddEvent(mouse_event);

				// Update currently focused object
				UpdateFocus(mouse_event->pos);
				break;
			}

		case sf::Event::KeyReleased:
		case sf::Event::KeyPressed:
			{
				KeyEvent *key_event = new KeyEvent();
				key_event->key = sf_event.key.code;
				switch (sf_event.type) {
				case sf::Event::KeyPressed:
					key_event->type = Event::E_KEY_PRESSED;
					break;
				case sf::Event::KeyReleased:
					key_event->type = Event::E_KEY_RELEASED;
					break;
				}
				AddEvent(key_event);
				break;
			}
		}
	}

	void EventManager::Update() {
		for (std::vector<Event*>::iterator iter = events_.begin(); iter != events_.end(); iter++) {
			Event *event = *iter;

			EventToFocusToListenerSetMap::iterator event_iter = eventfocus__listener_map_.find(event->type);
			if (event_iter == eventfocus__listener_map_.end()) {
				continue;
			}
			FocusToListenerSetMap *focus_listener_map = event_iter->second;

			// Send event to all listeners with NULL focus i.e. global focus
			FocusToListenerSetMap::iterator focus_iter = focus_listener_map->find(NULL);
			if (focus_iter != focus_listener_map->end()) {
				ListenerSet* listener_set = focus_iter->second;
				for (ListenerSet::iterator listener_iter = listener_set->begin();
						listener_iter != listener_set->end(); listener_iter++) {
					EventListener *listener = *listener_iter;
					listener->ProcessEvent(event);
				}
			}

			// Send event to all listeners on the current focused object if any
			if (focus_ != NULL) {
				focus_iter = focus_listener_map->find(focus_);
				if (focus_iter != focus_listener_map->end()) {
					ListenerSet* listener_set = focus_iter->second;
					for (ListenerSet::iterator listener_iter = listener_set->begin();
							listener_iter != listener_set->end(); listener_iter++) {
						EventListener *listener = *listener_iter;
						listener->ProcessEvent(event);
					}
				}
			}
		}
		events_.clear();
	}

	int ZSort(GameObject* a, GameObject* b) {
		return a->GetZ() - b->GetZ();
	}

	void EventManager::UpdateFocus(sf::Vector2i mouse) {
		static sf::Vector2i old_mouse = sf::Mouse::getPosition();

		// Find all intersecting focus objects
		std::vector<GameObject*> intersections;
		for (std::vector<GameObject*>::iterator focus_iter = game_objects_.begin(); focus_iter != game_objects_.end(); focus_iter++) {
			GameObject *object = *focus_iter;
			if (object->GetRect().contains((float)mouse.x, (float)mouse.y)) {
				intersections.push_back(object);
			}
		}
		// Sort by z-order
		GameObject *newfocus_ = NULL;
		if (intersections.size() > 0) {
			std::sort(intersections.begin(), intersections.end(), ZSort);
			newfocus_ = intersections.front();
		}

		// If new focus send enter and exit events
		if (newfocus_ != focus_) {
			MouseEvent mouse_event;
			if (focus_ != NULL) {
				// Exit event
				mouse_event.type = Event::E_MOUSE_EXITED;
				mouse_event.pos = old_mouse;
				EventToFocusToListenerSetMap::iterator event_iter = eventfocus__listener_map_.find(Event::E_MOUSE_EXITED);
				if (event_iter != eventfocus__listener_map_.end()) {
					FocusToListenerSetMap *focus_listener_map = event_iter->second;
					FocusToListenerSetMap::iterator focus_iter = focus_listener_map->find(focus_);
					if (focus_iter != focus_listener_map->end()) {
						ListenerSet* listener_set = focus_iter->second;
						for (ListenerSet::iterator listener_iter = listener_set->begin(); listener_iter != listener_set->end(); listener_iter++) {
							EventListener *listener = *listener_iter;
							listener->ProcessEvent(&mouse_event);
						}
					}
				}
			}
			focus_ = newfocus_;
			if (focus_ != NULL) {
				// Enter event
				mouse_event.type = Event::E_MOUSE_ENTERED;
				mouse_event.pos = mouse;
				EventToFocusToListenerSetMap::iterator event_iter = eventfocus__listener_map_.find(Event::E_MOUSE_ENTERED);
				if (event_iter != eventfocus__listener_map_.end()) {
					FocusToListenerSetMap *focus_listener_map = event_iter->second;
					FocusToListenerSetMap::iterator focus_iter = focus_listener_map->find(focus_);
					if (focus_iter != focus_listener_map->end()) {
						ListenerSet* listener_set = focus_iter->second;
						for (ListenerSet::iterator listener_iter = listener_set->begin(); listener_iter != listener_set->end(); listener_iter++) {
							EventListener *listener = *listener_iter;
							listener->ProcessEvent(&mouse_event);
						}
					}
				}
			}
		}
		old_mouse = mouse;
	}
}