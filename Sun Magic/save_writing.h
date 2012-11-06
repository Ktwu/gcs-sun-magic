#pragma once

#include "stdafx.h"
#include "character_tile.h"
#include "game.h"
#include "machine_state.h"
#include "machine_states.h"

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

		CharacterTile* _tile_;
		sf::Font _font_;
		sf::Text _prompt_;
		sf::Text _current_;
	};

}