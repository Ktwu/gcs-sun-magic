#include "stdafx.h"
#include "main_menu.h"

#include "game.h"
#include "gameasset_manager.h"
#include "texture_refs.h"
#include "tools_images.h"

namespace sun_magic {

	MainMenu::MainMenu() :
		background_(),
		game_state_(),
		play_(50, 50, 100, 100, "Play"),
		record_(300, 300, 100, 100, "Record")
	{
	}

	MainMenu::~MainMenu() {}

	void MainMenu::RegisterState(MachineState<ref::GameState>* previousState) {
		GameAssetManager* manager = GameAssetManager::GetInstance();
		background_.setTexture(*manager->GetTexture(texture_refs::tutorial::poster_close));
		/* The image might be a little too big, so scale it so it fits in the window */
		tools::images::ScaleToWindowSize(background_);

		game_state_ = ref::MAIN_MENU;
		Game::GetInstance()->GetEventManager()->AddGameObject(&play_);
		Game::GetInstance()->GetEventManager()->AddGameObject(&record_);
		Game::GetInstance()->GetEventManager()->RegisterListener(Event::E_MOUSE_RELEASED, this, &play_);
		Game::GetInstance()->GetEventManager()->RegisterListener(Event::E_MOUSE_RELEASED, this, &record_);
	}

	void MainMenu::UnregisterState(MachineState<ref::GameState>* previousState) {
		GameAssetManager* manager = GameAssetManager::GetInstance();
		manager->ReturnTexture(texture_refs::tutorial::poster_close);
		
		Game::GetInstance()->GetEventManager()->RemoveGameObject(&play_);
		Game::GetInstance()->GetEventManager()->RemoveGameObject(&record_);
		Game::GetInstance()->GetEventManager()->UnregisterListener(Event::E_MOUSE_RELEASED, this, &play_);
		Game::GetInstance()->GetEventManager()->RegisterListener(Event::E_MOUSE_RELEASED, this, &record_);
	}

	ref::GameState MainMenu::Update(float elapsed_time) {
		return game_state_;
	}
	
	void MainMenu::PreDraw(sf::RenderTarget *target) {
		Game::GetInstance()->GetWindow()->draw(background_);
	}

	void MainMenu::PostDraw(sf::RenderTarget *target) {
	}

	void MainMenu::ProcessEvent(Event *event) {
		switch(event->type) {
		case Event::E_MOUSE_RELEASED:
			if (event->focus == &play_) {
				game_state_ = ref::PLAYING;
			} else if (event->focus == &record_) {
				game_state_ = ref::RECORDING;
			}
			break;
		}
	}

};