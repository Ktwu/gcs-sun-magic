#include "stdafx.h"
#include "game.h"
#include "machine_state.h"
#include "red.h"
#include "sm_mouse.h"

namespace SunMagic {

	RedState::RedState() {}

	RedState::~RedState() {}

	void RedState::registerState(MachineState* previousState) {
	}

	Game::GameState RedState::update() {
		Game::mainWindow.clear(sf::Color::Red);
		return _handleInput();
		// So long as I don't return an actual state, this'll keep running.
	}

	void RedState::unregisterState(MachineState* previousState) {
	}

	Game::GameState RedState::_handleInput() {
		if (Game::mouse.haveButtonPressEvent &&
			Game::mouse.buttonPressEvent.button == sf::Mouse::Left) {
			std::cout << "Mouse pressed!\n";
			return  Game::s_Blue;
		}

		return Game::s_None;
	}

};