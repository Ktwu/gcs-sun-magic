#pragma once

#include "stdafx.h"
#include "events/gameevent.h"
#include "events/gameevent_manager.h"
	
namespace sun_magic {

	GameEventManager* GameEventManager::instance_ = NULL;

	GameEventManager::GameEventManager() {}
	GameEventManager::~GameEventManager() {}

	GameEventManager* GameEventManager::GetInstance() {
		if (instance_ == NULL) {
			instance_ = new GameEventManager();
		}
		return instance_;
	}

	bool GameEventManager::Set(GameEvent event, bool did_occur) {
		bool old = events_[event];
		events_[event] = did_occur;
		return old;
	}

	bool GameEventManager::Get(GameEvent event) {
		return events_[event];
	}

	int GameEventManager::NumOverlap(std::set<GameEvent> events) {
		int overlap = 0;
		for (std::set<GameEvent>::iterator i = events.begin(); i != events.end(); ++i) {
			if (events_[*i])
				++overlap;
		}
		return overlap;
	}

	bool GameEventManager::AreAllTrue(std::set<GameEvent> events) {
		return (NumOverlap(events) == events.size());
	}

	bool GameEventManager::AreAllFalse(std::set<GameEvent> events) {
		return (NumOverlap(events) == 0);
	}

	std::set<KeyObject*>& GameEventManager::GetKeyObjectsFor(MachineState<GameState>* state) {
		return key_objects_[state];
	}

	bool& GameEventManager::operator [](GameEvent event) {
		return events_[event];
	}

	std::set<KeyObject*>& GameEventManager::operator [](MachineState<GameState>* state) {
		return key_objects_[state];
	}

}