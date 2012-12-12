#include "stdafx.h"
#include "new_level.h"

#include "game.h"
#include "assets/gameasset_manager.h"
#include "events/event_manager.h"
#include "events/gameevent_manager.h"
#include "objects/animon.h"
#include "objects/progressbar.h"
#include "references/refs.h"
#include "tools/tools.h"

namespace sun_magic {

	NewLevelState::NewLevelState() {
		start_button_ = UiElement(0, 0, 200, 40, "Start");
		UiElement::InitButton(&start_button_);

		/* Treat the vector like a queue and push our levels on backwards */
		levels_.push_back(L"さしすせそ");
		levels_.push_back(L"かきくけこ");
		levels_.push_back(L"あいうえお");
		for (int i = 0; i < GameAssetManager::NUM_SYMBOLS; ++i)
			hiragana_scores_[GameAssetManager::hiragana_strings[i]] = 0;

		for (int i = 0; i < 5; ++i)
			intro_display_.UiAdd(UiElement::InitLabel(new UiElement(0, 0, 0, 0, "--")));
		intro_display_.UiAdd(&start_button_);
	}

	NewLevelState::~NewLevelState() {
		intro_display_.UiRemove(&start_button_);
		for (int i = 0; i < 5; ++i) {
			delete intro_display_[i];
		}
		intro_display_.UiClear();
	}

	bool NewLevelState::MapSort(std::pair<sf::String, int> a, std::pair<sf::String, int> b) {
		return a.second < b.second;
	}

	sf::String NewLevelState::GetNewLevelHiragana() {
		level_hiragana_.clear();
		if (!levels_.empty()) {
			level_hiragana_ = levels_[levels_.size() - 1];
			levels_.pop_back();
		} else {
			/* Explore our map and find the 5 weakest hiragana */
			std::vector<std::pair<sf::String, int>> sorted_map(hiragana_scores_.size());
			std::map<sf::String, int>::iterator i;
			int j;
			for (i = hiragana_scores_.begin(), j = 0; i != hiragana_scores_.end(); ++i, ++j)
				sorted_map[j] = *i;
			std::sort(sorted_map.begin(), sorted_map.end(), NewLevelState::MapSort);

			// Add the 5 weakest hiragana we found.
			for (j = 0; j < 5; ++j) {
				level_hiragana_ += sorted_map[j].first;
				++hiragana_scores_[sorted_map[j].first];
			}
		}

		return level_hiragana_;
	}
	sf::String NewLevelState::GetCurrentLevelHiragana() {
		return level_hiragana_;
	}

	void NewLevelState::RegisterState(MachineState<GameState>* previous_state) {
		state_ = GameState::NEW_LEVEL_LOAD;
		std::cout << "NewLevelState TIME!\n";

		GameAssetManager* asset_manager = GameAssetManager::GetInstance();
		background_.setTexture(*asset_manager->GetTexture(this, refs::textures::backgrounds::OFFICE));
		// The image might be a little too big, so scale it so it fits in the window
		tools::ScaleToWindowSize(background_);

		// Load our game's UI
		//Game::GetInstance()->AddUIElements();
		sf::Vector2u size = Game::GetInstance()->GetWindow()->getSize();

		intro_display_.SetSize(sf::Vector2f(700, 400));
		sf::Vector2f pos = intro_display_.GetSize();
		pos.x = (size.x - pos.x)/2;
		pos.y = (size.y - pos.y)/2;
		intro_display_.SetPosition(pos);
		
		// Load up the hiragana we're going to be teaching for this level.
		GetNewLevelHiragana();

		float width = intro_display_.GetSize().x / 5.f;
		sf::Font* font = GameAssetManager::GetInstance()->GetFont(this, refs::fonts::MSMINCHO);
		size_t i;
		for (i = 0; i < level_hiragana_.getSize(); ++i) {
			UiElement* label = (UiElement*) intro_display_[i];
			label->GetStyle()->SetTextFont(*font)->SetTextColor(sf::Color::Blue)->SetTextSize(60)
				->SetNormalBorderColor(sf::Color::Transparent)->SetNormalColor(sf::Color::Transparent);
			label->SetPosition(sf::Vector2f(width*i, 0));
			label->SetSize(sf::Vector2f(width, width));
			label->SetString(level_hiragana_[i]);
		}
		for (; i < intro_display_.UiSize()-1; ++i) {
			UiElement* label = (UiElement*) intro_display_[i];
			label->SetSize(sf::Vector2f(0,0));
			label->SetString("");
		}

		start_button_.SetPosition(sf::Vector2f(0, width));
		start_button_.GetStyle()->SetAllowPress(true);
		EventManager* manager = Game::GetInstance()->GetEventManager();
		manager->AddGameObject(&intro_display_);
		manager->RegisterListener(Event::E_CLICKED, this, &start_button_);
	}

	void NewLevelState::UnregisterState(MachineState<GameState>* next_state) {
		GameAssetManager* asset_manager = GameAssetManager::GetInstance();
		asset_manager->ReturnTextures(this);
		asset_manager->ReturnFonts(this);

		EventManager* manager = Game::GetInstance()->GetEventManager();
		manager->RemoveGameObject(&intro_display_);
		manager->UnregisterListener(Event::E_CLICKED, this, &start_button_);
	}

	GameState NewLevelState::Update(float elapsed_time) {
		return state_;
	}
	
	void NewLevelState::PreDraw(sf::RenderTarget *target) {
		target->draw(background_);
	}

	void NewLevelState::PostDraw(sf::RenderTarget *target) {
	}

	void NewLevelState::ProcessEvent(Event event) {
		switch (event.type) {
		case Event::E_GAME_EVENT:
			if (event.gameEvent == GameEvent::ANIMON_RIGHT)
				++hiragana_scores_[event.message[0]];
			else
				--hiragana_scores_[event.message[0]];
			break;
		case Event::E_CLICKED:
			state_ = GameState::FEEDING;
		}
		
	}

};