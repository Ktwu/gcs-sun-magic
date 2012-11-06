#include "stdafx.h"
#include "game.h"

#include "blue.h"
#include "game_states.h"
#include "machine.h"
#include "machine_state.h"
#include "red.h"
#include "save_writing.h"
#include "time.h"

#include <fstream>
#include <iostream>

namespace sun_magic {

	Game* Game::instance_ = NULL;

	Game::Game() {}

	Game::~Game() {}

	sf::RenderWindow* Game::GetWindow() {
		return &main_window_;
	}

	EventManager* Game::GetEventManager() {
		return &event_manager_;
	}

	void Game::Init() {
		game_state_ = ref::PLAYING;
		main_window_.create(sf::VideoMode(1024, 768, 32), "Sun Magic!");

		// Init zinnia data
		// TODO replace with a better dataset
		CharacterTile::InitRecognizer("zinnia/models/hiragana.model");

		//game_machine_.Init(ref::BLUE, new BlueState());
		//game_machine_.AddState(ref::RED, new RedState());
		game_machine_.Init(ref::MachineStates::RED, new SaveWritingState());
		
		event_manager_.RegisterListener(Event::E_CLOSED, this);
		event_manager_.RegisterListener(Event::E_KEY_RELEASED, this);
	}

	void Game::HandleInput() {
		sf::Event event;
		while (main_window_.pollEvent(event)) {
			event_manager_.AddEvent(event);
		}
		event_manager_.Update();
	}

	void Game::Run() {	
		clock_t last_clock = clock();
		float time_since_last_update;
		while(game_state_ != ref::EXITING) {
			clock_t new_clock = clock();
			time_since_last_update = (float)(new_clock - last_clock) / CLOCKS_PER_SEC;
			last_clock = new_clock;
		
			HandleInput();
			game_machine_.Update(time_since_last_update);
			main_window_.display();
		}
	}

	void Game::Destroy() {
		main_window_.close();
	}

	void Game::ProcessEvent(Event *event) {
		switch (event->type) {
		case Event::E_KEY_RELEASED:
			{
				KeyEvent *key_event = (KeyEvent*)event;
				switch (key_event->key) {
				case Keyboard::Escape:
					game_state_ = ref::EXITING;
					break;
				}
				break;
			}

		case Event::E_CLOSED:
			game_state_ = ref::EXITING;
			break;
		}
	}
}