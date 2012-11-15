#pragma once

#include "stdafx.h"
#include "states/game_state.h"
#include "states/machine_state.h"

namespace sun_magic {

	class Splash : public MachineState<GameState> {
	public:
		Splash();
		~Splash();

		/* Interface functions */
		void RegisterState(MachineState<GameState>* previous_state);
		void UnregisterState(MachineState<GameState>* previous_state);

		GameState Update(float elapsed_time);
		void PreDraw(sf::RenderTarget *target);
		void PostDraw(sf::RenderTarget *target);

		void LoadAssets();

	private:
		sf::Sprite background_;
		sf::Text text_;
		GameState game_state_;
		float text_cumulated_time_;
		int text_state_;
	};

}