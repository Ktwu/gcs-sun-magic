#pragma once

#include "stdafx.h"
#include "game.h"

#include <fstream>
#include <iostream>
#include "time.h"

// Game states
#include "assets/gameasset_manager.h"
#include "events/event_manager.h"
#include "references/file_refs.h"
#include "states/splash.h"
#include "states/main_menu.h"
#include "states/save_writing.h"
#include "states/playing.h"
#include "ui/character_tile.h"
#include "ui/dictionary.h"

namespace sun_magic {

	Game* Game::instance_ = NULL;

	Game::Game() {}

	Game::~Game() {
		delete event_manager_;
	}

	sf::RenderWindow* Game::GetWindow() {
		return &main_window_;
	}

	EventManager* Game::GetEventManager() {
		return event_manager_;
	}

	void Game::Init() {
		GameAssetManager* asset_manager = GameAssetManager::GetInstance();
		asset_manager->Init();

		game_state_ = PLAYING;
		main_window_.create(sf::VideoMode(1024, 768, 32), "Sun Magic!");

		// Init zinnia data
		// TODO replace with a better dataset
		CharacterTile::InitRecognizer(file_refs::ZINNIA_MODEL);

		game_machine_.Init(LOADING, new Splash());
		game_machine_.AddState(MAIN_MENU, new MainMenu());
		game_machine_.AddState(RECORDING, new SaveWritingState());
		game_machine_.AddState(PLAYING, new Playing());
		
		event_manager_ = new EventManager();
		event_manager_->RegisterListener(Event::E_CLOSED, this);
		event_manager_->RegisterListener(Event::E_KEY_RELEASED, this);

		dict_ = new Dictionary(0,0,0,0);
		words_.push_back("hello world");
		words_.push_back("blah");
	}

	void Game::HandleInput() {
		sf::Event event;
		while (main_window_.pollEvent(event)) {
			event_manager_->AddEvent(event);
		}
		event_manager_->Update();
	}

	void Game::Run() {	
		clock_t last_clock = clock();
		float time_since_last_update;
		while(game_state_ != EXITING) {
			clock_t new_clock = clock();
			time_since_last_update = (float)(new_clock - last_clock) / CLOCKS_PER_SEC;
			last_clock = new_clock;
		
			HandleInput();

			// Update
			event_manager_->UpdateObjects(time_since_last_update);
			game_machine_.Update(time_since_last_update);

			// Draw
			MachineState<GameState> *state = game_machine_.GetActiveState();
			state->PreDraw(&main_window_);
			event_manager_->DrawObjects(&main_window_);
			dict_->Draw(&main_window_); 
		    dict_->DictWords(&main_window_, words_);
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
					game_state_ = EXITING;
					break;
				}
				break;
			}

		case Event::E_CLOSED:
			game_state_ = EXITING;
			break;
		}
	}
}