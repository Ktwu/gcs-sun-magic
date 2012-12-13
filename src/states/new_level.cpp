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
		selected_index_(0)
	{
		sf::RenderWindow* window = Game::GetInstance()->GetWindow();
		sf::Vector2u size = window->getSize();

		float padding = 10;
		float temp_y = 0;
		
		// init ui group
		UiElement::InitLabel(&intro_display_);
		intro_display_.SetPosition(sf::Vector2f(size.x * .25f, 0.f));
		intro_display_.SetSize(sf::Vector2f(size.x * .6f, (float)size.y));
		intro_display_.GetStyle()->SetNormalColor(sf::Color(100, 100, 100));

		// init label for level
		UiElement::InitLabel(&level_label_);
		level_label_.GetStyle()->SetNormalColor(sf::Color::Cyan);
		level_label_.SetSize(sf::Vector2f(intro_display_.GetSize().x, 60));
		level_label_.SetString("Meet Your Animons!");
		intro_display_.UiAdd(&level_label_);
		temp_y += level_label_.GetSize().y + padding;

		UiElement::InitLabel(&tip_label_);
		tip_label_.GetStyle()->SetTextSize(18);
		tip_label_.SetPosition(sf::Vector2f(0, temp_y));
		tip_label_.SetSize(sf::Vector2f(intro_display_.GetSize().x, 60));
		tip_label_.SetString("Try writing your animons' nicknames! Stroke order matters!\nLeft click and drag to draw. Right click to undo bad strokes.\n");
		intro_display_.UiAdd(&tip_label_);
		temp_y += tip_label_.GetSize().y + padding;

		// init character tile for tracing
		tile_.SetPosition(sf::Vector2f(tools::Center(intro_display_.GetSize().x, tile_.GetSize().x), temp_y));
		tile_.GetTileStyle()->SetGuideColor(sf::Color(235, 235, 235))->SetBorderColor(sf::Color(190, 190, 190))->SetErrorColor(sf::Color::Red);
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

		float temp_width = 90;
		float temp_padding = (intro_display_.GetSize().x - temp_width * NUM_HIRAGANA_LABELS) / (NUM_HIRAGANA_LABELS+1);
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
		temp_y += hiragana_labels_[0].GetSize().y + padding;

		UiElement::InitLabel(&about_label_);
		about_label_.SetPosition(sf::Vector2f(0, temp_y));
		about_label_.SetSize(sf::Vector2f(intro_display_.GetSize().x, 80));
		about_label_.GetStyle()->SetTextSize(20);
		intro_display_.UiAdd(&about_label_);
		temp_y += about_label_.GetSize().y + padding;
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
		tip_label_.GetStyle()->SetTextFont(*english_font);
		start_button_.GetStyle()->SetTextFont(*english_font)->SetIsVisible(false)->SetIsEnabled(false);

		// Load up the hiragana we're going to be teaching for this level.
		GetNewLevelHiragana();
		int width = intro_display_.GetSize().x / 5.f;
		num_hiragana_ = level_hiragana_.getSize();

		sf::Texture *texture = asset_manager->GetTexture(this, refs::textures::objects::SPRITES_MEH);
		int i;
		for (i = 0; i < num_hiragana_; ++i) {
			sf::Sprite sprite = asset_manager->GetHiraganaSprite(level_hiragana_[i], texture);
			sf::FloatRect sprite_bounds = sprite.getLocalBounds();
			sf::Vector2f label_size = hiragana_labels_[i].GetSize();
			sprite.setScale(label_size.x / sprite_bounds.width, label_size.y / sprite_bounds.height);
			
			hiragana_done_[i] = false;
			UiElement::InitButton(hiragana_labels_ + i);
			hiragana_labels_[i].GetStyle()->SetTextColor(sf::Color::Transparent)->SetTextFont(*font)->SetIsEnabled(true)->SetIsVisible(true)
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
		about_label_.GetStyle()->SetTextFont(*english_font);
		
		SelectLabel(0);

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
				for (int i = 0; i < num_hiragana_; i++) {
					if (event.focus == hiragana_labels_ + i) {
						SelectLabel(i);
					}
				}
			}
			break;
		case Event::E_HIRAGANA_DRAWN:
			UiElement *label = hiragana_labels_ + selected_index_;
			if (event.message == label->GetString()) {
				GameAssetManager* manager = GameAssetManager::GetInstance();

				if (!hiragana_done_[selected_index_]) {
					hiragana_done_[selected_index_] = true;
					sf::Color color = manager->symbols_colors[(manager->GetHiraganaIndex(event.message[0]))];
					label->GetStyle()->SetNormalColor(color)
						->SetHoverColor(tools::Multiply(color, 0.8f))
						->SetPressColor(tools::Multiply(color, 0.6f));

					int i = 0;
					for (; i < num_hiragana_; i++) {
						if (!hiragana_done_[i]) {
							break;
						}
					}
					if (i < num_hiragana_) {
						SelectLabel(i);
					} else {
						start_button_.GetStyle()->SetIsVisible(true)->SetIsEnabled(true);
					}
				}
			}
			break;
		}	
	}

	void NewLevelState::SelectLabel(int index) {
		GameAssetManager* manager = GameAssetManager::GetInstance();

		// Reset old selected hiragana
		hiragana_labels_[selected_index_].GetStyle()->SetIsEnabled(true);
		hiragana_labels_[selected_index_].SetState(UiElement::UiElementState::NEUTRAL);

		// Set new selected hiragan
		hiragana_labels_[index].GetStyle()->SetIsEnabled(false);
		hiragana_labels_[index].SetState(UiElement::UiElementState::PRESSED);
		selected_index_ = index;

		int hiragana_index = manager->GetHiraganaIndex(hiragana_labels_[index].GetString());
		about_label_.SetString(GameAssetManager::about_strings[hiragana_index]);

		tile_.Clear();
		tile_.SetTraceCharacter(manager->GetTraceCharacter(hiragana_labels_[index].GetString()));
		tile_.SetAnimationStroke(0);
	}

};