#pragma once

#include "stdafx.h"
#include "events/event.h"
#include "states/game_state.h"
#include "states/machine_state.h"
#include "ui/character_tilelist.h"

namespace sun_magic {

	class Playing : public MachineState<GameState>, public EventListener {
	public:
		Playing();
		~Playing();

		/* Interface functions */
		void RegisterState(MachineState<GameState>* previous_state);
		void UnregisterState(MachineState<GameState>* next_state);

		GameState Update(float elapsed_time);
		void PreDraw(sf::RenderTarget *target);
		void PostDraw(sf::RenderTarget *target);

		void ProcessEvent(Event event);

	private:
		CharacterTileList tilelist_;
		sf::Sprite background_;
		GameState game_state_;
	};

}