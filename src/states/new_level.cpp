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

	NewLevelState::NewLevelState() :
		selected_hiragana_(NULL)
	{
		sf::RenderWindow* window = Game::GetInstance()->GetWindow();
		sf::Vector2u size = window->getSize();

		float padding = 20;
		float temp_y = padding;
		
		// init ui group
		UiElement::InitLabel(&intro_display_);
		intro_display_.SetPosition(sf::Vector2f(size.x * .25f, 0.f));
		intro_display_.SetSize(sf::Vector2f(size.x * .5f, (float)size.y));
		intro_display_.GetStyle()->SetNormalColor(sf::Color(100, 100, 100));

		// init label for level
		UiElement::InitLabel(&level_label_);
		level_label_.GetStyle()->SetNormalColor(sf::Color::Cyan);
		level_label_.SetSize(sf::Vector2f(intro_display_.GetSize().x, 60));
		level_label_.SetString("New Animons!");
		intro_display_.UiAdd(&level_label_);
		temp_y += level_label_.GetSize().y + padding;

		// init character tile for tracing
		tile_.SetPosition(sf::Vector2f(tools::Center(intro_display_.GetSize().x, tile_.GetSize().x), temp_y));
		tile_.GetTileStyle()->SetGuideColor(sf::Color(235, 235, 235))->SetBorderColor(sf::Color(190, 190, 190));
		tile_.GetStyle()->SetNormalColor(sf::Color::White);
		intro_display_.UiAdd(&tile_);
		temp_y += tile_.GetSize().y + padding;

		UiElement::InitButton(&start_button_);
		start_button_.SetSize(sf::Vector2f(200, 40));
		start_button_.SetPosition(sf::Vector2f(tools::Center(intro_display_.GetSize().x, 200), temp_y));
		start_button_.SetString("START!");
		intro_display_.UiAdd(&start_button_);
		temp_y += start_button_.GetSize().y + padding;

		/* Treat the vector like a queue and push our levels on backwards */
		levels_.push_back(L"さしすせそ");
		levels_.push_back(L"かきくけこ");
		levels_.push_back(L"あいうえお");
		for (int i = 0; i < GameAssetManager::NUM_SYMBOLS; ++i)
			hiragana_scores_[GameAssetManager::hiragana_strings[i]] = 0;

		float temp_padding = 5;
		float temp_width = (intro_display_.GetSize().x - temp_padding) / NUM_HIRAGANA_LABELS - temp_padding;
		float temp_x = temp_padding;

		for (int i = 0; i < NUM_HIRAGANA_LABELS; ++i) {
			UiElement* label = &hiragana_labels_[i];
			UiElement::InitButton(label);
			label->GetStyle()->SetTextColor(sf::Color::Transparent)
				->SetNormalColor(sf::Color::White);

			label->SetPosition(sf::Vector2f(temp_x, temp_y));
			label->SetSize(sf::Vector2f(temp_width, temp_width));

			intro_display_.UiAdd(&hiragana_labels_[i]);
			temp_x += label->GetSize().x + temp_padding;
		}
	}

	NewLevelState::~NewLevelState() {
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

		EventManager* manager = Game::GetInstance()->GetEventManager();
		GameAssetManager* asset_manager = GameAssetManager::GetInstance();

		background_.setTexture(*asset_manager->GetTexture(this, refs::textures::backgrounds::OFFICE));
		tools::ScaleToWindowSize(background_);

		sf::Vector2u size = Game::GetInstance()->GetWindow()->getSize();

		sf::Font* font = GameAssetManager::GetInstance()->GetFont(this, refs::fonts::KAORI);
		sf::Font* english_font = GameAssetManager::GetInstance()->GetFont(this, refs::fonts::MSMINCHO);

		// Set the font for common UI elements
		level_label_.GetStyle()->SetTextFont(*english_font);
		start_button_.GetStyle()->SetTextFont(*english_font)->SetIsVisible(false)->SetIsEnabled(false);

		// Load up the hiragana we're going to be teaching for this level.
		GetNewLevelHiragana();
		int width = intro_display_.GetSize().x / 5.f;
		num_hiragana_left_ = level_hiragana_.getSize();

		sf::Texture *texture = asset_manager->GetTexture(this, refs::textures::objects::SPRITES_MEH);
		int i;
		for (i = 0; i < num_hiragana_left_; ++i) {
			sf::Sprite sprite = asset_manager->GetHiraganaSprite(level_hiragana_[i], texture);
			sprite.setScale(0.7, 0.7);		

			hiragana_labels_[i].GetStyle()->SetTextFont(*font)->SetIsEnabled(true)->SetIsVisible(true)
				->SetNormalSprite(sprite)->SetHoverSprite(sprite)->SetPressSprite(sprite)
				->SetNormalColor(sf::Color::White);
			hiragana_labels_[i].SetString(level_hiragana_[i]);

			manager->RegisterListener(Event::E_CLICKED, this, &hiragana_labels_[i]);
		}
		for (; i < NUM_HIRAGANA_LABELS; ++i) {
			hiragana_labels_[i].GetStyle()->SetTextFont(sf::Font())->SetIsEnabled(false)->SetIsVisible(false)
				->SetNormalColor(sf::Color::White);
			hiragana_labels_[i].SetString("");
		}

		// Add Listeners
		manager->AddGameObject(&intro_display_);
		manager->RegisterListener(Event::E_CLICKED, this, &start_button_);
		manager->RegisterListener(Event::E_HIRAGANA_DRAWN, this, &tile_);
	}

	void NewLevelState::UnregisterState(MachineState<GameState>* next_state) {
		GameAssetManager* asset_manager = GameAssetManager::GetInstance();
		asset_manager->ReturnTextures(this);
		asset_manager->ReturnFonts(this);

		EventManager* manager = Game::GetInstance()->GetEventManager();
		manager->RemoveGameObject(&intro_display_);

		manager->UnregisterListener(Event::E_CLICKED, this, &start_button_);
		manager->UnregisterListener(Event::E_HIRAGANA_DRAWN, this, &tile_);

		for (int i = 0; i < NUM_HIRAGANA_LABELS; ++i)
			manager->UnregisterListener(Event::E_CLICKED, this, &hiragana_labels_[i]);

		selected_hiragana_ = NULL;
		tile_.SetTraceCharacter(NULL);
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
			if (event.focus == &start_button_)
				state_ = GameState::FEEDING;
			else {
				GameAssetManager* manager = GameAssetManager::GetInstance();
				tile_.Clear();
				tile_.SetTraceCharacter(manager->GetTraceCharacter(event.message[0]));
				tile_.SetAnimationStroke(0);
				selected_hiragana_ = (UiElement*) event.focus;
			}
			break;
		case Event::E_HIRAGANA_DRAWN:
			if (selected_hiragana_ != NULL && event.message == selected_hiragana_->GetString()) {
				--num_hiragana_left_;

				GameAssetManager* manager = GameAssetManager::GetInstance();
				selected_hiragana_->GetStyle()->SetIsEnabled(false)->SetNormalColor(manager->symbols_colors[(manager->GetHiraganaIndex(event.message[0]))]);

				if (num_hiragana_left_ == 0)
					start_button_.GetStyle()->SetIsVisible(true)->SetIsEnabled(true);
			}
			break;
		}	
	}

};