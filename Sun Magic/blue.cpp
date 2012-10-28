#include "stdafx.h"
#include "blue.h"
#include "game.h"
#include "machine_state.h"
#include "sm_mouse.h"

namespace sun_magic {

	BlueState::BlueState() {}

	BlueState::~BlueState() {}

	void BlueState::RegisterState(MachineState<ref::MachineStates>* previousState) {
	}

	ref::MachineStates BlueState::Update() {
		Game::GetInstance()->main_window_.clear(sf::Color::Blue);
		return HandleInput();
		// So long as I don't return an actual state, this'll keep running.
	}

	void BlueState::UnregisterState(MachineState<ref::MachineStates>* previousState) {
	}

	ref::MachineStates BlueState::HandleInput() {
		Game* game = Game::GetInstance();
		if (game->mouse_.haveButtonPressEvent &&
			game->mouse_.buttonPressEvent.button == sf::Mouse::Left) {
			std::cout << "Mouse pressed!\n";
			return  ref::RED;
		}

		return ref::NONE;
	}
};