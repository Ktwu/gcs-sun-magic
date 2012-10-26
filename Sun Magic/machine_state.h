#pragma once

#include "stdafx.h"

namespace SunMagic {

	template <class T>
	class MachineState {
	public:
		MachineState() {}
		~MachineState() {}

		virtual void registerState(MachineState<T>* previousState) = 0;
		virtual T update() = 0;
		virtual void unregisterState(MachineState<T>* previousState) = 0;
	};

}