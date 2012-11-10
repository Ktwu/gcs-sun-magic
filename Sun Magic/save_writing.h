#pragma once

#include "stdafx.h"
#include "character_tile.h"
#include "game.h"
#include "machine_state.h"
#include "machine_states.h"

#include <fstream>

namespace sun_magic {

	class SaveWritingState : public MachineState<ref::MachineStates>, public EventListener {
	public:
		SaveWritingState();
		~SaveWritingState();

		/* Interface functions */
		void RegisterState(MachineState<ref::MachineStates>* previous_state);
		ref::MachineStates Update(float elapsed_time);
		void UnregisterState(MachineState<ref::MachineStates>* previous_state);

	private:
		void UpdateText();
		void ProcessEvent(Event *event);

		sf::String target_hiragana;
		int target_index;

		static const int size = 1024;
		char buff[size];
		bool _have_trace_;

		CharacterTile* _tile_;
		sf::Font _font_;
		sf::Text _prompt_;
		sf::Text _current_;
		sf::Text _save_;
		std::ofstream _trace_output_;
		zinnia::Character *_character_;
	};

}