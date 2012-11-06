#pragma once

#include "stdafx.h"
#include "events.h"
#include "machine_state.h"
#include "machine_states.h"

namespace sun_magic {

	class RedState : public MachineState<ref::MachineStates>, public EventListener {
	public:
		RedState();
		~RedState();

		/* Interface functions */
		void RegisterState(MachineState<ref::MachineStates>* previous_state);
		void UnregisterState(MachineState<ref::MachineStates>* previous_state);
		ref::MachineStates Update(float elapsed_time);

	private:
		sf::Sprite _background_;
		bool change_;

		void ProcessEvent(Event *event);
	};

}