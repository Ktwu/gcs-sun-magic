#include "stdafx.h"
#include "blue.h"
#include "game.h"
#include "machine_state.h"
#include "sm_mouse.h"

namespace SunMagic {

	BlueState::BlueState() {}

	BlueState::~BlueState() {}

	void BlueState::registerState(MachineState* previousState) {
	}

	Game::GameState BlueState::update() {
		Game::mainWindow.clear(sf::Color::Blue);
		return _handleInput();
		// So long as I don't return an actual state, this'll keep running.
	}

	void BlueState::unregisterState(MachineState* previousState) {
	}

	Game::GameState BlueState::_handleInput() {
		if (Game::mouse.haveButtonPressEvent &&
			Game::mouse.buttonPressEvent.button == sf::Mouse::Left) {
			std::cout << "Mouse pressed!\n";
			return  Game::s_Red;
		}

		return Game::s_None;
	}
};