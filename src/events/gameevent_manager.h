#pragma once

#include "stdafx.h"
#include "events/gameevent.h"
#include "states/machine_state.h"
	
namespace sun_magic {
	class GameEventManager {
	public:
		GameEventManager();
		~GameEventManager();

		static GameEventManager* GetInstance();

		bool Set(GameEvent event, bool did_occur);
		bool Get(GameEvent event);

		int NumOverlap(std::set<GameEvent> events);
		bool AreAllTrue(std::set<GameEvent> events);
		bool AreAllFalse(std::set<GameEvent> events);

		bool& operator [](GameEvent event);

	private:
		static GameEventManager* instance_;
		bool events_[GameEvent::NUM_EVENTS];
	};
}