#include "stdafx.h"
#include "feeding.h"

#include "game.h"
#include "assets/gameasset_manager.h"
#include "events/event_manager.h"
#include "events/gameevent_manager.h"
#include "objects/animon.h"
#include "objects/progressbar.h"
#include "references/refs.h"
#include "tools/tools.h"

namespace sun_magic {

	Feeding::Feeding() :
		background_(),
		hiraganas_(),
		animons_(),
		progressbars_(),
		game_state_(),
		feed_increment_(0.35f),
		eat_rate_(0.01f),
		happy_threshold_(0.7f),
		ok_threshold_(0.4f)
	{
	}

	Feeding::~Feeding() {}

	void Feeding::SetHiraganas(std::vector<sf::String> hiraganas) {
		hiraganas_ = hiraganas;
	}

	void Feeding::RegisterState(MachineState<GameState>* previous_state) {
		game_state_ = FEEDING;

		GameAssetManager* asset_manager = GameAssetManager::GetInstance();
		background_.setTexture(*asset_manager->GetTexture(refs::textures::backgrounds::OFFICE));
		// The image might be a little too big, so scale it so it fits in the window
		tools::ScaleToWindowSize(background_);

		// Load our game's UI
		Game::GetInstance()->AddUIElements();
		sf::Vector2u size = Game::GetInstance()->GetWindow()->getSize();

		// Create animon objects
		Dictionary *dictionary = Game::GetInstance()->GetDictionary();
		EventManager* event_manager = Game::GetInstance()->GetEventManager();
		event_manager->RegisterListener(Event::E_HIRAGANA_DRAWN, this);
		float y = 350.f;

		float width = (size.x - 50) / 5;
		for (size_t i = 0; i < hiraganas_.size(); i++) {
			sf::String h = hiraganas_[i];

			Animon *animon = new Animon((float)i * width, y, GameAssetManager::symbols_colors[asset_manager->GetHiraganaIndex(h[0])], h);
			animon->LoadState(Animon::AnimonState::MEH);
			animon->WaitForLoadState();
			sf::Sprite sprite = animon->GetSprite();

			animon->SetZ(1);
			animons_.push_back(animon);
			event_manager->AddGameObject(animon);
			event_manager->RegisterListener(Event::E_GAME_EVENT, this, animon);

			ProgressBar *progressbar = new ProgressBar((i + 0.3f) * width, 0, y + sprite.getLocalBounds().height, refs::textures::objects::FOODBOWL, refs::textures::objects::SUSHI);
			progressbar->SetZ(10);
			progressbars_.push_back(progressbar);
			event_manager->AddGameObject(progressbar);

			dictionary->AddWord(h, sprite);
		}

		Event load_event;
		load_event.type = Event::E_LOAD_STATE;
		event_manager->AddEvent(load_event);
	}

	void Feeding::UnregisterState(MachineState<GameState>* next_state) {
		GameAssetManager* manager = GameAssetManager::GetInstance();
		manager->ReturnTexture(refs::textures::backgrounds::WINDOW);

		Game::GetInstance()->RemoveUIElements();

		EventManager* event_manager = Game::GetInstance()->GetEventManager();
		event_manager->UnregisterListener(Event::E_HIRAGANA_DRAWN, this);
		for (size_t i = 0; i < animons_.size(); i++) {
			Animon *animon = animons_[i];
			event_manager->RemoveGameObject(animon);
			event_manager->UnregisterListener(Event::E_GAME_EVENT, this, animon);
		}
	}

	GameState Feeding::Update(float elapsed_time) {
		bool all_happy = true;
		GameAssetManager* manager = GameAssetManager::GetInstance();

		for (size_t i = 0; i < animons_.size(); i++) {
			ProgressBar *progressbar = progressbars_[i];
			float progress = progressbar->GetProgress();
			progress -= eat_rate_ * elapsed_time;
			progressbar->SetProgress(progress);

			Animon *animon = animons_[i];
			// TODO: Set animon state sprite
			if (progress >= happy_threshold_) {
				animon->LoadState(Animon::AnimonState::HAPPY);
			} else if (progress >= ok_threshold_) {
				animon->LoadState(Animon::AnimonState::MEH);
				all_happy = false;
			} else {
				animon->LoadState(Animon::AnimonState::ANGRY);
				all_happy = false;
			}
		}

		if (all_happy)
			game_state_ = MAIN_MENU;
		return game_state_;
	}
	
	void Feeding::PreDraw(sf::RenderTarget *target) {
		target->draw(background_);
	}

	void Feeding::PostDraw(sf::RenderTarget *target) {
	}

	void Feeding::ProcessEvent(Event event) {
		static sf::String hiragana = "";

		switch (event.type) {
		case Event::E_HIRAGANA_DRAWN:
			hiragana = event.message;
			break;
		case Event::E_GAME_EVENT:
			if (event.gameEvent == GameEvent::KEY_CLICK && hiragana.getSize() > 0) {
				size_t i = 0;
				for (; i < animons_.size(); i++) {
					if (animons_[i] == event.source)
						break;
				}
				if (i == animons_.size())
					break;

				if (event.message == hiragana) {
					ProgressBar *progressbar = progressbars_[i];
					progressbar->SetProgress(progressbar->GetProgress() + feed_increment_);
				} else {
					ProgressBar *progressbar = progressbars_[i];
					progressbar->SetProgress(progressbar->GetProgress() - feed_increment_);
				}
				Game::GetInstance()->GetTileList()->Clear();
				hiragana = "";
			}
			break;
		}
	}

};