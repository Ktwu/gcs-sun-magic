#include "stdafx.h"
#include "main_menu.h"

#include "game.h"
#include "assets/gameasset_manager.h"
#include "events/event_manager.h"
#include "references/refs.h"
#include "tools/tools.h"
#include "ui/style.h"

namespace sun_magic {

	MainMenu::MainMenu() :
		background_(),
		game_state_(),
		animate_state_(-1)
	{
		sf::RenderWindow* window = Game::GetInstance()->GetWindow();

		/* Go ahead and just stick our UI code in here */
		float width = 400;
		play_ = UiElement((window->getSize().x - width) / 2, 500.f, width, 50.f, "Play");
		record_ = UiElement((window->getSize().x - width) / 2, 600.f, width, 50.f, "Record");
		UiElement::InitButton(&play_);
		UiElement::InitButton(&record_);

		play_.GetStyle()
			->SetNormalColor(sf::Color::Transparent)->SetNormalBorderColor(sf::Color::Transparent)
			->SetHoverColor(sf::Color::Transparent)->SetHoverBorderColor(sf::Color::Transparent)
			->SetPressColor(sf::Color::Transparent)->SetPressBorderColor(sf::Color::Transparent);
		record_.GetStyle()
			->SetNormalColor(sf::Color::Transparent)->SetNormalBorderColor(sf::Color::Transparent)
			->SetHoverColor(sf::Color::Transparent)->SetHoverBorderColor(sf::Color::Transparent)
			->SetPressColor(sf::Color::Transparent)->SetPressBorderColor(sf::Color::Transparent);
	}

	MainMenu::~MainMenu() {
		GameAssetManager* manager = GameAssetManager::GetInstance();
	}

	void MainMenu::RegisterState(MachineState<GameState>* previous_state) {
		GameAssetManager* manager = GameAssetManager::GetInstance();

		sf::Font* font = manager->GetFont(this, refs::fonts::MSMINCHO);

		// Load background
		background_.setTexture(*manager->GetTexture(this, refs::textures::backgrounds::TITLE));
		tools::ScaleToWindowSize(background_);

		// Load button textures
		std::vector<sf::Texture*> textures;
		std::vector<std::string> files;
		files.push_back(refs::textures::ui::BUTTON);
		files.push_back(refs::textures::ui::BUTTON_HOVER);
		files.push_back(refs::textures::ui::BUTTON_PRESS);
		manager->GetTextures(this, files, textures);

		// YAY NO ERROR CHECKING.  DERP.
		sf::Sprite normal = sf::Sprite(*textures[0]);
		sf::Sprite hover = sf::Sprite(*textures[1]);
		sf::Sprite press = sf::Sprite(*textures[2]);

		play_.GetStyle()->SetNormalSprite(normal)->SetHoverSprite(hover)->SetPressSprite(press)->SetTextFont(*font);
		record_.GetStyle()->SetNormalSprite(normal)->SetHoverSprite(hover)->SetPressSprite(press)->SetTextFont(*font);

		game_state_ = MAIN_MENU;
		EventManager* event_manager = Game::GetInstance()->GetEventManager();
		event_manager->AddGameObject(&play_);
		event_manager->AddGameObject(&record_);
		event_manager->RegisterListener(Event::E_CLICKED, this, &play_);
		event_manager->RegisterListener(Event::E_CLICKED, this, &record_);
	}

	void MainMenu::UnregisterState(MachineState<GameState>* next_state) {
		GameAssetManager* manager = GameAssetManager::GetInstance();
		manager->ReturnTextures(this);
		manager->ReturnFonts(this);
		
		EventManager* event_manager = Game::GetInstance()->GetEventManager();
		event_manager->RemoveGameObject(&play_);
		event_manager->RemoveGameObject(&record_);
		event_manager->UnregisterListener(Event::E_CLICKED, this, &play_);
		event_manager->RegisterListener(Event::E_CLICKED, this, &record_);
	}

	GameState MainMenu::Update(float elapsed_time) {
		return game_state_;
	}
	
	void MainMenu::PreDraw(sf::RenderTarget *target) {
		target->draw(background_);
	}

	void MainMenu::PostDraw(sf::RenderTarget *target) {

		/* Test animation block for fade-to-black */
		switch(animate_state_) {
		case 0:
			animate_timer_.restart();
			++animate_state_;
			break;
		case 1:
			sf::Color black = sf::Color::Black;
			float time = animate_timer_.getElapsedTime().asMilliseconds();
			black.a = sf::Uint8(255 * (time / 1000.f));

			if (time >= 1000.f) {
				animate_state_ = -1;
				game_state_ = after_state_;
			}

			sf::Vector2u size = target->getSize();
			sf::RectangleShape temp(sf::Vector2f((float)size.x, (float)size.y));
			temp.setFillColor(black);
			target->draw(temp);
			break;
		}
	}

	void MainMenu::ProcessEvent(Event event) {
		switch(event.type) {
		case Event::E_CLICKED:
			if (event.focus == &play_) {
				//game_state_ = NEW_LEVEL_LOAD;
				after_state_ = NEW_LEVEL_LOAD;
				animate_state_ = 0;
			} else if (event.focus == &record_) {
				//game_state_ = RECORDING;
				after_state_ = RECORDING;
				animate_state_ = 0;
			}
			break;
		}
	}

}