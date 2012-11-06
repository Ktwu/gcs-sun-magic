#include "stdafx.h"
#include "blue.h"
#include "game.h"
#include "game_states.h"
#include "machine.h"
#include "machine_state.h"
#include "red.h"
#include "save_writing.h"
#include "sm_mouse.h"
#include "time.h"

#include <fstream>
#include <iostream>

namespace sun_magic {

	Game* Game::instance_ = NULL;

	void Game::Init() {
		game_state_ = ref::PLAYING;
		main_window_.create(sf::VideoMode(1024, 768, 32), "Sun Magic!");

		//game_machine_.Init(ref::BLUE, new BlueState());
		//game_machine_.AddState(ref::RED, new RedState());
		game_machine_.Init(ref::MachineStates::RED, new SaveWritingState());

		// Load font data
		if (!font_.loadFromFile("msmincho.ttc")) {
			std::cerr << "ERROR: Unable to load font msmincho.ttc." << std::endl;
		}

		// Init zinnia data
		// TODO replace with a better dataset
		CharacterTile::InitRecognizer("zinnia/models/hiragana.model");
	}

	void Game::HandleInput() {
		sf::Event event;
		mouse_.reset();

		while (main_window_.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				game_state_ = ref::EXITING;

			if (!mouse_.haveButtonPressEvent) {
				mouse_.haveButtonPressEvent = event.type == sf::Event::MouseButtonPressed;
				mouse_.buttonPressEvent = event.mouseButton;
			}

			if (!mouse_.haveButtonReleaseEvent) {
				mouse_.haveButtonReleaseEvent = event.type == sf::Event::MouseButtonReleased;
				mouse_.buttonReleaseEvent = event.mouseButton;
			}

			if (!mouse_.haveMoveEvent) {
				mouse_.haveMoveEvent = event.type == sf::Event::MouseMoved;
				mouse_.moveEvent = event.mouseMove;
			}

			if (!mouse_.haveWheelEvent) {
				mouse_.haveWheelEvent = event.type == sf::Event::MouseWheelMoved;
				mouse_.wheelEvent = event.mouseWheel;
			}
		}
	}

	void Game::Run() {	
		clock_t last_clock = clock();
		while(game_state_ != ref::EXITING) {
			clock_t new_clock = clock();
			time_since_last_update = (float)(new_clock - last_clock) / CLOCKS_PER_SEC;
			last_clock = new_clock;
		
			HandleInput();
			game_machine_.Update();
			main_window_.display();
		}
	}

	void Game::Destroy() {
		main_window_.close();
	}

}