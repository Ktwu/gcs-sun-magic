#pragma once

#include "stdafx.h"
#include "events/event.h"
#include "states/game_state.h"
#include "states/machine_state.h"
#include "ui/ui_element.h"

namespace sun_magic {

	class MainMenu : public MachineState<GameState>, public EventListener {
	public:
		MainMenu();
		~MainMenu();

		/* Interface functions */
		void RegisterState(MachineState<GameState>* previous_state);
		void UnregisterState(MachineState<GameState>* next_state);

		GameState Update(float elapsed_time);
		void PreDraw(sf::RenderTarget *target);
		void PostDraw(sf::RenderTarget *target);

		void ProcessEvent(Event event);

	private:
		sf::Sprite background_;
		GameState game_state_;
		UiElement play_;
		UiElement record_;
		UiElement credits_;

		int animate_state_;
		GameState after_state_;
		sf::Clock animate_timer_;
	};

}