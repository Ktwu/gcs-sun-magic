#pragma once

#include "stdafx.h"
#include "machine_state.h"
#include "machine_states.h"

namespace sun_magic {

	class BlueState : public MachineState<ref::MachineStates> {
	public:
		BlueState();
		~BlueState();

		/* Interface functions */
		void RegisterState(MachineState<ref::MachineStates>* previous_state);
		ref::MachineStates Update();
		void UnregisterState(MachineState<ref::MachineStates>* previous_state);

	private:
		sf::Sprite _background_;

		ref::MachineStates HandleInput();
	};

}