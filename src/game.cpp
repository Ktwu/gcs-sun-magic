#pragma once

#include "stdafx.h"
#include "game.h"

#include <fstream>
#include <iostream>

// Game states
#include "assets/gameasset_manager.h"
#include "events/event_manager.h"
#include "references/refs.h"
#include "states/splash.h"
#include "states/main_menu.h"
#include "states/new_level.h"
#include "states/save_writing.h"
#include "states/feeding.h"
#include "tools/tools.h"
#include "ui/character_tile.h"
#include "ui/dictionary.h"

namespace sun_magic {

	Game* Game::instance_ = NULL;

	Game::Game() {}

	Game::~Game() {
		GameAssetManager::GetInstance()->ReturnTextures(this);
		GameAssetManager::GetInstance()->ReturnFonts(this);
		delete event_manager_;
	}

	sf::RenderWindow* Game::GetWindow() {
		return &main_window_;
	}

	EventManager* Game::GetEventManager() {
		return event_manager_;
	}

	Machine<GameState>* Game::GetMachine() {
		return &game_machine_;
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

	UiElement* Game::GetTileListLabel() {
		return listlabel_;
	}

	void Game::Init() {
		GameAssetManager* asset_manager = GameAssetManager::GetInstance();
		asset_manager->Init();

		game_state_ = FEEDING;
		main_window_.create(sf::VideoMode(1024, 700/*768*/, 32), "Sun Magic!");

		// Init zinnia data
		CharacterTile::InitRecognizer(refs::zinnia::ZINNIA_MODEL.c_str());

		// Initialize event listeners
		event_manager_ = new EventManager();
		event_manager_->RegisterListener(Event::E_CLOSED, this);
		event_manager_->RegisterListener(Event::E_KEY_RELEASED, this);
		event_manager_->RegisterListener(Event::E_HIRAGANA_DRAWN, this);

		// Init UI elements
		sf::Vector2u size = main_window_.getSize();
		float height = 300;
		tilelist_ = new CharacterTileList(750.f - height, size.y - height, height, height, 100, 100, 1);
		tilelist_->SetZ(-1);
		tilelist_->GetStyle()->SetNormalSprite(sf::Sprite(*asset_manager->GetTexture(this, refs::textures::ui::NOTE_PAD)));

		listlabel_ = UiElement::InitLabel(new UiElement(750.f, size.y - height, size.x - 750.f, height));
		listlabel_->GetStyle()->SetTextFont(*asset_manager->GetFont(this, refs::fonts::KAORI))->SetTextSize(50);
		listlabel_->SetZ(-1);

		dict_ = new Dictionary(size.x - 20.f, 0, 800.f, 0, size.x - 800.f, size.y);
		dict_->SetZ(-2);

		// Init game states
		game_machine_.Init(MAIN_MENU, new MainMenu());
		game_machine_.AddState(RECORDING, new SaveWritingState());
		game_machine_.AddState(NEW_LEVEL_LOAD, new NewLevelState());
		game_machine_.AddState(FEEDING, new Feeding());
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