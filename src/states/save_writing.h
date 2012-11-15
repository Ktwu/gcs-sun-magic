#pragma once

#include "stdafx.h"
#include "game.h"
#include "events/event.h"
#include "states/game_state.h"
#include "states/machine_state.h"

namespace sun_magic {

	class CharacterTile;

	class SaveWritingState : public MachineState<GameState>, public EventListener {
	public:
		SaveWritingState();
		~SaveWritingState();

		/* Interface functions */
		void RegisterState(MachineState<GameState>* previous_state);
		void UnregisterState(MachineState<GameState>* previous_state);

		GameState Update(float elapsed_time);
		void PreDraw(sf::RenderTarget *target);
		void PostDraw(sf::RenderTarget *target);

		void ProcessEvent(Event *event);

	private:
		void UpdateText();

		sf::String target_hiragana;
		int target_index;

		static const int size = 1024;
		char buff[size];
		bool have_trace_;

		CharacterTile* tile_;
		sf::Font font_;
		sf::Text prompt_;
		sf::Text current_;
		sf::Text save_;
		std::ofstream trace_output_;
		zinnia::Character *character_;
	};

}