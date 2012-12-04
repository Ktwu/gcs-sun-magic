#include "stdafx.h"
#include "main_menu.h"

#include "game.h"
#include "assets/gameasset_manager.h"
#include "events/event_manager.h"
#include "references/refs.h"
#include "tools/tools.h"
#include "ui/button.h"
#include "ui/style.h"

namespace sun_magic {

	MainMenu::MainMenu() :
		background_(),
		game_state_()
	{
		sf::RenderWindow* window = Game::GetInstance()->GetWindow();

		/* Go ahead and just stick our UI code in here */
		float width = 400;
		play_ = new Button((window->getSize().x - width) / 2, 500.f, width, 50.f, "Play");
		record_ = new Button((window->getSize().x - width) / 2, 600.f, width, 50.f, "Record");

		play_->GetStyle()->SetTextColor(sf::Color::Black);
		record_->GetStyle()->SetTextColor(sf::Color::Black);
	}

	MainMenu::~MainMenu() {
		delete play_;
		delete record_;
	}

	void MainMenu::RegisterState(MachineState<GameState>* previous_state) {
		GameAssetManager* manager = GameAssetManager::GetInstance();
		background_.setTexture(*manager->GetTexture(refs::textures::backgrounds::POSTER_CLOSE));
		/* The image might be a little too big, so scale it so it fits in the window */
		tools::ScaleToWindowSize(background_);

		game_state_ = MAIN_MENU;
		EventManager* event_manager = Game::GetInstance()->GetEventManager();
		event_manager->AddGameObject(play_);
		event_manager->AddGameObject(record_);
		event_manager->RegisterListener(Event::E_CLICKED, this, play_);
		event_manager->RegisterListener(Event::E_CLICKED, this, record_);
	}

	void MainMenu::UnregisterState(MachineState<GameState>* next_state) {
		GameAssetManager* manager = GameAssetManager::GetInstance();
		manager->ReturnTexture(refs::textures::backgrounds::POSTER_CLOSE);
		
		EventManager* event_manager = Game::GetInstance()->GetEventManager();
		event_manager->RemoveGameObject(play_);
		event_manager->RemoveGameObject(record_);
		event_manager->UnregisterListener(Event::E_CLICKED, this, play_);
		event_manager->RegisterListener(Event::E_CLICKED, this, record_);

		manager->ReturnTexture(refs::textures::objects::NEKO);
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
				game_state_ = FEEDING;
			} else if (event.focus == record_) {
				game_state_ = RECORDING;
			}
			break;
		}
	}

}