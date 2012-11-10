#include "stdafx.h"
#include "game.h"

#include "game_state.h"
#include "machine.h"
#include "time.h"

// Game states
#include "splash.h"
#include "main_menu.h"
#include "save_writing.h"
#include "playing.h"

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

		game_machine_.Init(ref::LOADING, new Splash());
		game_machine_.AddState(ref::MAIN_MENU, new MainMenu());
		game_machine_.AddState(ref::RECORDING, new SaveWritingState());
		game_machine_.AddState(ref::PLAYING, new Playing());
		
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

			// Update
			game_machine_.Update(time_since_last_update);
			event_manager_.UpdateObjects(time_since_last_update);

			// Draw
			MachineState<ref::GameState> *state = game_machine_.GetActiveState();
			state->PreDraw(&main_window_);
			event_manager_.DrawObjects(&main_window_);
			state->PostDraw(&main_window_);
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