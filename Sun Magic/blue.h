#pragma once

#include "stdafx.h"
#include "events.h"
#include "machine_state.h"
#include "machine_states.h"

namespace sun_magic {

	class BlueState : public MachineState<ref::MachineStates>, public EventListener {
	public:
		BlueState();
		~BlueState();

		/* Interface functions */
		void RegisterState(MachineState<ref::MachineStates>* previous_state);
		void UnregisterState(MachineState<ref::MachineStates>* previous_state);
		ref::MachineStates Update();

	private:
		sf::Sprite _background_;
		bool change_;

		ref::MachineStates HandleInput();

		void ProcessEvent(Event *event);
	};

}