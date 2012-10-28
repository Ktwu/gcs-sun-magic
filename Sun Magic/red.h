#pragma once

#include "stdafx.h"
#include "machine_state.h"
#include "machine_states.h"

namespace sun_magic {

	class RedState : public MachineState<ref::MachineStates> {
	public:
		RedState();
		~RedState();

		/* Interface functions */
		void RegisterState(MachineState<ref::MachineStates>* previous_state);
		ref::MachineStates Update();
		void UnregisterState(MachineState<ref::MachineStates>* previous_state);

	private:
		ref::MachineStates HandleInput();
	};

}