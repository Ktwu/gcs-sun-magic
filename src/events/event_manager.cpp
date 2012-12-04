#include "stdafx.h"
#include "event_manager.h"

#include "objects/game_object.h"
#include "tools/tools.h"

namespace sun_magic {

	EventManager::EventManager() {}
	EventManager::~EventManager() {
		ClearListeners();
	}

	GameObject* EventManager::GetFocus() {
		return focus_;
	}

	void EventManager::AddGameObject(GameObject *object) {
		std::vector<GameObject*>::iterator focus_iter = game_objects_.begin();
		while (focus_iter != game_objects_.end() && (*focus_iter)->GetZ() > object->GetZ())
			focus_iter++;

		game_objects_.insert(focus_iter, object);
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

	void EventManager::ClearGameObjects() {
		for (int i = 0; i < game_objects_.size(); i++) {
			GameObject * object = game_objects_[i];
			object->Unregister();
		}
		game_objects_.clear();
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

	void EventManager::AddEvent(Event event) {
		events_.push_back(event);
	}

	void EventManager::AddEvent(sf::Event sf_event) {
		Event event;
		event.focus = NULL;

		switch (sf_event.type) {

		case sf::Event::Closed:
			{
				event.type = Event::E_CLOSED;
				AddEvent(event);
				break;
			}

		case sf::Event::MouseButtonPressed:
		case sf::Event::MouseButtonReleased:
			{
				switch (sf_event.type) {
				case sf::Event::MouseButtonPressed:
					event.type = Event::E_MOUSE_PRESSED;
					break;
				case sf::Event::MouseButtonReleased:
					event.type = Event::E_MOUSE_RELEASED;
					break;
				}
				event.mouseButton = sf_event.mouseButton;
				AddEvent(event);
				break;
			}
		case sf::Event::MouseMoved:
			{
				event.type = Event::E_MOUSE_MOVED;
				event.mouseMove = sf_event.mouseMove;
				AddEvent(event);

				// Update currently focused object
				UpdateFocus(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
				break;
			}
			
		case sf::Event::KeyPressed:
		case sf::Event::KeyReleased:
			{
				switch (sf_event.type) {
				case sf::Event::KeyPressed:
					event.type = Event::E_KEY_PRESSED;
					break;
				case sf::Event::KeyReleased:
					event.type = Event::E_KEY_RELEASED;
					break;
				}
				event.key = sf_event.key;
				AddEvent(event);
				break;
			}
		case sf::Event::TextEntered:
			{
				event.type = Event::E_TEXT_ENTERED;
				event.text = sf_event.text;
				AddEvent(event);
				break;
			}
		}
	}

	void EventManager::Update() {
		for (size_t i = 0; i < events_.size(); ++i) {
			Event event = events_[i];

			EventToFocusToListenerSetMap::iterator event_iter = eventfocus__listener_map_.find(event.type);
			if (event_iter == eventfocus__listener_map_.end()) {
				continue;
			}
			FocusToListenerSetMap *focus_listener_map = event_iter->second;

			// Send event to all listeners with NULL focus i.e. global focus
			event.focus = (event.focus != NULL) ? event.focus : focus_;
			FocusToListenerSetMap::iterator focus_iter = focus_listener_map->find(NULL);
			if (focus_iter != focus_listener_map->end()) {
				ListenerSet* listener_set = focus_iter->second;
				for (ListenerSet::iterator listener_iter = listener_set->begin();
						listener_iter != listener_set->end(); listener_iter++) {
					EventListener *listener = *listener_iter;
					listener->ProcessEvent(event);
				}
			}

			// Send event to all listeners on the currently focused object if any
			if (event.focus != NULL) {
				focus_iter = focus_listener_map->find(event.focus);
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

	void EventManager::UpdateObjects(float elapsed_time) {
		for (int i = 0; i < game_objects_.size(); i++) {
			GameObject * object = game_objects_[i];
			object->Update(elapsed_time);
		}
	}

	void EventManager::DrawObjects(sf::RenderTarget *target) {
		// Save the window default view
		sf::View view = target->getView();

		for (int i = 0; i < game_objects_.size(); i++) {
			GameObject * object = game_objects_[i];

			// Translate view to object position
			view.move(object->GetNegativePosition());
			target->setView(view);
			object->Draw(target);
			view.move(object->GetPosition());
		}

		// Reset window view
		target->setView(view);
	}


	void EventManager::UpdateFocus(sf::Vector2i mouse) {
		static sf::Vector2i old_mouse = sf::Mouse::getPosition();

		// Find the first intersection focus object. Objects in the back are nearer the front.
		GameObject *newfocus_ = NULL;
		for (int i = game_objects_.size() - 1; i >= 0; i--) {
			GameObject *object = game_objects_[i];
			if (object->GetRect().contains((float)mouse.x, (float)mouse.y)) {
				newfocus_ = object;
				if (newfocus_ != focus_) {
					if (focus_ == Game::GetInstance()->GetDictionary()) {
						int a = 1;
					}
				}
				break;
			}
		}

		// If new focus send enter and exit events
		if (newfocus_ != focus_) {
			if (focus_ != NULL) {
				// Exit event
				Event event;
				event.type = Event::E_MOUSE_EXITED;
				event.focus = focus_;
				event.mouseMove.x = old_mouse.x;
				event.mouseMove.y = old_mouse.y;
				EventToFocusToListenerSetMap::iterator event_iter = eventfocus__listener_map_.find(Event::E_MOUSE_EXITED);
				if (event_iter != eventfocus__listener_map_.end()) {
					FocusToListenerSetMap *focus_listener_map = event_iter->second;
					FocusToListenerSetMap::iterator focus_iter = focus_listener_map->find(focus_);
					if (focus_iter != focus_listener_map->end()) {
						ListenerSet* listener_set = focus_iter->second;
						for (ListenerSet::iterator listener_iter = listener_set->begin(); listener_iter != listener_set->end(); listener_iter++) {
							EventListener *listener = *listener_iter;
							listener->ProcessEvent(event);
						}
					}
				}
			}
			focus_ = newfocus_;
			if (focus_ != NULL) {
				// Enter event
				Event event;
				event.type = Event::E_MOUSE_ENTERED;
				event.focus = focus_;
				event.mouseMove.x = mouse.x;
				event.mouseMove.y = mouse.y;
				EventToFocusToListenerSetMap::iterator event_iter = eventfocus__listener_map_.find(Event::E_MOUSE_ENTERED);
				if (event_iter != eventfocus__listener_map_.end()) {
					FocusToListenerSetMap *focus_listener_map = event_iter->second;
					FocusToListenerSetMap::iterator focus_iter = focus_listener_map->find(focus_);
					if (focus_iter != focus_listener_map->end()) {
						ListenerSet* listener_set = focus_iter->second;
						for (ListenerSet::iterator listener_iter = listener_set->begin(); listener_iter != listener_set->end(); listener_iter++) {
							EventListener *listener = *listener_iter;
							listener->ProcessEvent(event);
						}
					}
				}
			}
		}
		old_mouse = mouse;
	}

}