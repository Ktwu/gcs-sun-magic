#include "stdafx.h"
#include "machine_state.h"
#include "red.h"
#include "sm_mouse.h"
#include "game.h"

namespace sun_magic {

	RedState::RedState() {}

	RedState::~RedState() {}

	void RedState::RegisterState(MachineState<ref::MachineStates>* previousState) {
	}

	ref::MachineStates RedState::Update() {
		Game::GetInstance()->main_window_.clear(sf::Color::Blue);
		return HandleInput();
		// So long as I don't return an actual state, this'll keep running.
	}

	void RedState::UnregisterState(MachineState<ref::MachineStates>* previousState) {
	}

	ref::MachineStates RedState::HandleInput() {
		Game* game = Game::GetInstance();
		if (game->mouse_.haveButtonPressEvent &&
			game->mouse_.buttonPressEvent.button == sf::Mouse::Left) {
			std::cout << "Mouse pressed!\n";
			return  ref::BLUE;
		}

		return ref::NONE;
	}
};