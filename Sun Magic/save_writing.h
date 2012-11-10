#pragma once

#include "stdafx.h"
#include "character_tile.h"
#include "game.h"
#include "machine_state.h"

#include <fstream>

namespace sun_magic {

	class SaveWritingState : public MachineState<ref::GameState>, public EventListener {
	public:
		SaveWritingState();
		~SaveWritingState();

		/* Interface functions */
		void RegisterState(MachineState<ref::GameState>* previous_state);
		void UnregisterState(MachineState<ref::GameState>* previous_state);

		ref::GameState Update(float elapsed_time);
		void PreDraw(sf::RenderTarget *target);
		void PostDraw(sf::RenderTarget *target);

		void ProcessEvent(Event *event);

	private:
		void UpdateText();

		sf::String target_hiragana;
		int target_index;

		static const int size = 1024;
		char buff[size];
		bool _have_trace_;

		std::ofstream _trace_output_;
		CharacterTile* _tile_;
		sf::Font _font_;
		sf::Text _prompt_;
		sf::Text _current_;
		sf::Text _save_;
	};

}