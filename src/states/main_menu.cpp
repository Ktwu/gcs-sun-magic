#include "stdafx.h"
#include "main_menu.h"

#include "game.h"
#include "assets/gameasset_manager.h"
#include "events/event_manager.h"
#include "references/texture_refs.h"
#include "tools/tools.h"
#include "ui/button.h"

namespace sun_magic {

	MainMenu::MainMenu() :
		background_(),
		game_state_(),
		play_(new Button(50, 50, 100, 100, "Play")),
		record_(new Button(300, 300, 100, 100, "Record"))
	{
	}

	MainMenu::~MainMenu() {
		delete play_;
		delete record_;
	}

	void MainMenu::RegisterState(MachineState<GameState>* previous_state) {
		GameAssetManager* manager = GameAssetManager::GetInstance();
		background_.setTexture(*manager->GetTexture(textures::backgrounds::POSTER_CLOSE));
		/* The image might be a little too big, so scale it so it fits in the window */
		tools::ScaleToWindowSize(background_);

		game_state_ = MAIN_MENU;
		Game::GetInstance()->GetEventManager()->AddGameObject(play_);
		Game::GetInstance()->GetEventManager()->AddGameObject(record_);
		Game::GetInstance()->GetEventManager()->RegisterListener(Event::E_CLICKED, this, play_);
		Game::GetInstance()->GetEventManager()->RegisterListener(Event::E_CLICKED, this, record_);
	}

	void MainMenu::UnregisterState(MachineState<GameState>* next_state) {
		GameAssetManager* manager = GameAssetManager::GetInstance();
		manager->ReturnTexture(textures::backgrounds::POSTER_CLOSE);
		
		Game::GetInstance()->GetEventManager()->RemoveGameObject(play_);
		Game::GetInstance()->GetEventManager()->RemoveGameObject(record_);
		Game::GetInstance()->GetEventManager()->UnregisterListener(Event::E_CLICKED, this, play_);
		Game::GetInstance()->GetEventManager()->RegisterListener(Event::E_CLICKED, this, record_);
	}

	GameState MainMenu::Update(float elapsed_time) {
		return game_state_;
	}
	
	void MainMenu::PreDraw(sf::RenderTarget *target) {
		Game::GetInstance()->GetWindow()->draw(background_);
	}

	void MainMenu::PostDraw(sf::RenderTarget *target) {}

	void MainMenu::ProcessEvent(Event event) {
		switch(event.type) {
		case Event::E_CLICKED:
			if (event.focus == play_) {
				game_state_ = PLAYING;
			} else if (event.focus == record_) {
				game_state_ = RECORDING;
			}
			break;
		}
	}

}