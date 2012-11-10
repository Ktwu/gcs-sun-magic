#pragma once

#include "stdafx.h"
#include "game_state.h"
#include "machine_state.h"

namespace sun_magic {

	class Splash : public MachineState<ref::GameState> {
	public:
		Splash();
		~Splash();

		/* Interface functions */
		void RegisterState(MachineState<ref::GameState>* previous_state);
		void UnregisterState(MachineState<ref::GameState>* previous_state);

		ref::GameState Update(float elapsed_time);
		void PreDraw(sf::RenderTarget *target);
		void PostDraw(sf::RenderTarget *target);

		void LoadAssets();

	private:
		sf::Sprite background_;
		sf::Text text_;
		ref::GameState game_state_;
		float text_cumulated_time_;
		int text_state_;
	};

}