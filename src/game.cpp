#pragma once

#include "stdafx.h"
#include "game.h"

#include <fstream>
#include <iostream>

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
		sf::Clock clock;
		while(game_state_ != EXITING) {
			float elapsed_time = clock.getElapsedTime().asSeconds();
			clock.restart();
		
			HandleInput();

			// Update
			event_manager_->UpdateObjects(elapsed_time);
			game_machine_.Update(elapsed_time);

			// Draw
			MachineState<GameState> *state = game_machine_.GetActiveState();
			state->PreDraw(&main_window_);
			event_manager_->DrawObjects(&main_window_);
			dict_->Draw(&main_window_); 
		    dict_->DictWords(&main_window_, words_);
			state->PostDraw(&main_window_);
			main_window_.display();
		}

		// Cleanup
		game_machine_.GetActiveState()->UnregisterState(NULL);
	}

	void Game::Destroy() {
		main_window_.close();
	}

	void Game::ProcessEvent(Event event) {
		switch (event.type) {
		case Event::E_KEY_RELEASED:
			{
				switch (event.key.code) {
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