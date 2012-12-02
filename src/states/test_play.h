#pragma once

#include "stdafx.h"
#include "events/event.h"
#include "objects/animon.h"
#include "states/game_state.h"
#include "states/machine_state.h"
#include "ui/character_tilelist.h"
#include "ui/label.h"
#include "ui/ui_group.h"

namespace sun_magic {

	class TestPlay : public MachineState<GameState>, public EventListener {
	public:
		TestPlay();
		~TestPlay();

		/* Interface functions */
		void RegisterState(MachineState<GameState>* previous_state);
		void UnregisterState(MachineState<GameState>* next_state);

		GameState Update(float elapsed_time);
		void PreDraw(sf::RenderTarget *target);
		void PostDraw(sf::RenderTarget *target);

		void ProcessEvent(Event event);

	private:
		void RegisterAnimon(Animon* animon);
		void UnregisterAnimon(Animon* animon);

		sf::Sprite background_;
		GameState game_state_;

		Animon* selected_animon_;

		UiGroup test_group;
		Label test_label;
	};

}