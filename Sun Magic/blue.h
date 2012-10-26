#pragma once

#include "stdafx.h"
#include "game.h"
#include "machine_state.h"

namespace SunMagic {

	class BlueState : public MachineState<Game::GameState> {
	public:
		BlueState();
		~BlueState();

		// Interface functions
		void registerState(MachineState<Game::GameState>* previousState);
		Game::GameState update();
		void unregisterState(MachineState<Game::GameState>* previousState);

	private:

		Game::GameState _handleInput();
	};

}