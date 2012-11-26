#pragma once

#include "stdafx.h"
#include "game.h"

#include <fstream>
#include <iostream>

// Game states
#include "assets/gameasset_manager.h"
#include "events/event_manager.h"
#include "references/file_refs.h"
#include "references/texture_refs.h"
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

	void Game::AddUIElements() {
		event_manager_->AddGameObject(tilelist_);
		event_manager_->AddGameObject(dict_);
		event_manager_->AddGameObject(listlabel_);
	}

	void Game::RemoveUIElements() {
		event_manager_->RemoveGameObject(tilelist_);
		event_manager_->RemoveGameObject(dict_);
		event_manager_->RemoveGameObject(listlabel_);
	}

	Dictionary* Game::GetDictionary() {
		return dict_;
	}

	CharacterTileList* Game::GetTileList() {
		return tilelist_;
	}

	void Game::Init() {
		GameAssetManager* asset_manager = GameAssetManager::GetInstance();
		asset_manager->Init();

		game_state_ = PLAYING;
		main_window_.create(sf::VideoMode(1024, 725, 32), "Sun Magic!");

		// Init zinnia data
		// TODO replace with a better dataset
		CharacterTile::InitRecognizer(file_refs::ZINNIA_MODEL.c_str());

		game_machine_.Init(LOADING, new Splash());
		game_machine_.AddState(MAIN_MENU, new MainMenu());
		game_machine_.AddState(RECORDING, new SaveWritingState());
		game_machine_.AddState(PLAYING, new Playing());
		
		event_manager_ = new EventManager();
		event_manager_->RegisterListener(Event::E_CLOSED, this);
		event_manager_->RegisterListener(Event::E_KEY_RELEASED, this);
		event_manager_->RegisterListener(Event::E_HIRAGANA_DRAWN, this);

		sf::Vector2u size = main_window_.getSize();
		int height = 150;
		tilelist_ = new CharacterTileList(0, size.y - height, 750, height, 5);

		dict_ = new Dictionary(750, size.y - height - 40, 750, 0, size.x - 750, size.y);
		listlabel_ = new Label(750, size.y - height, size.x - 750, height);
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
				switch (event.key.code) {
				case Keyboard::Escape:
					game_state_ = EXITING;
					break;
				}
				break;

		case Event::E_CLOSED:
			game_state_ = EXITING;
			break;

		case Event::E_HIRAGANA_DRAWN:
			listlabel_->SetString(event.message);
			break;
		}
	}
}