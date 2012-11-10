#pragma once

#include "stdafx.h"
#include "game_state.h"
#include "machine_state.h"

namespace sun_magic {

	class Playing : public MachineState<ref::GameState> {
	public:
		Playing();
		~Playing();

		/* Interface functions */
		void RegisterState(MachineState<ref::GameState>* previous_state);
		void UnregisterState(MachineState<ref::GameState>* previous_state);

		ref::GameState Update(float elapsed_time);
		void PreDraw(sf::RenderTarget *target);
		void PostDraw(sf::RenderTarget *target);

	private:
		sf::Sprite background_;
		ref::GameState game_state_;
	};

}