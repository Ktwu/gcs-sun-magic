#pragma once

#include <stdafx.h>

#include "events/event.h"
#include "states/machine_state.h"
#include "ui/ui_group.h"

namespace sun_magic {

	class CreditsState : public MachineState<GameState>, public EventListener {
	public:
		static sf::String labels_[];

		CreditsState();
		~CreditsState();

		void RegisterState(MachineState<GameState>* previous_state);
		void UnregisterState(MachineState<GameState>* next_state);

		GameState Update(float elapsed_time);
		void PreDraw(sf::RenderTarget *target);
		void PostDraw(sf::RenderTarget *target);

		void ProcessEvent(sun_magic::Event);

	private:
		GameState game_state_;
		sf::Sprite background_;

		UiGroup credits_group_;
		UiElement menu_button_;
	};

}