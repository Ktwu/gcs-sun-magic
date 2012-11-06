#pragma once

#include "stdafx.h"

namespace sun_magic {

	template <class T>
	class MachineState {
	public:
		MachineState() {}
		~MachineState() {}

		virtual void RegisterState(MachineState<T>* previous_state) = 0;
		virtual void UnregisterState(MachineState<T>* previous_state) = 0;
		virtual T Update(float elapsed_time) = 0;
	};

}