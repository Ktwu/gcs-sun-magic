#pragma once

#include "stdafx.h"
#include "events/event.h"
#include "states/game_state.h"
#include "states/machine_state.h"

namespace sun_magic {

	class Button;

	class MainMenu : public MachineState<GameState>, public EventListener {
	public:
		MainMenu();
		~MainMenu();

		/* Interface functions */
		void RegisterState(MachineState<GameState>* previous_state);
		void UnregisterState(MachineState<GameState>* previous_state);

		GameState Update(float elapsed_time);
		void PreDraw(sf::RenderTarget *target);
		void PostDraw(sf::RenderTarget *target);

		void ProcessEvent(Event *event);

	private:
		sf::Sprite background_;
		GameState game_state_;
		Button *play_;
		Button *record_;
	};

}