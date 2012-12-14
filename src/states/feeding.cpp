#include "stdafx.h"
#include "feeding.h"

#include "game.h"
#include "assets/gameasset_manager.h"
#include "events/event_manager.h"
#include "events/gameevent_manager.h"
#include "objects/animon.h"
#include "objects/progressbar.h"
#include "references/refs.h"
#include "states/new_level.h"
#include "tools/tools.h"
#include "ui/card.h"

namespace sun_magic {

	const int MAX_ANIMONS = 5;

	Feeding::Feeding() :
		background_(),
		hiraganas_(),
		animons_(),
		progressbars_(),
		game_state_(),
		feed_increment_(0.6f),
		eat_rate_(0.006f),
		happy_threshold_(0.7f),
		ok_threshold_(0.4f)
	{
		sf::RenderWindow* window = Game::GetInstance()->GetWindow();
		sf::Vector2u size = window->getSize();

		UiElement::InitLabel(&info_label_);
		info_label_.GetStyle()->SetTextSize(20)->SetNormalColor(sf::Color::Magenta);
		info_label_.SetPosition(sf::Vector2f(0, 0));
		info_label_.SetSize(sf::Vector2f(size.x, 70));
		info_label_.SetString("Your animons are hungry! Make sure all of them are happy and well fed!\nFeed them by writing their names in the notebook then clicking on them.\nAnimons gain a glow around their head when they are well fed!");
	}

	Feeding::~Feeding() {}

	void Feeding::RegisterState(MachineState<GameState>* previous_state) {
		game_state_ = FEEDING;
		
		EventManager* event_manager = Game::GetInstance()->GetEventManager();
		GameAssetManager* asset_manager = GameAssetManager::GetInstance();
		background_.setTexture(*asset_manager->GetTexture(this, refs::textures::backgrounds::DESK));//refs::textures::backgrounds::OFFICE));
		tools::ScaleToWindowSize(background_);

		// Load our game's UI
		sf::Vector2u size = Game::GetInstance()->GetWindow()->getSize();

		// Load up the hiragana designated by some other state
		NewLevelState* state = (NewLevelState*) Game::GetInstance()->GetMachine()->GetState(GameState::NEW_LEVEL_LOAD);
		hiraganas_ = state->GetCurrentLevelHiragana();
		
		sf::Font* english_font = GameAssetManager::GetInstance()->GetFont(this, refs::fonts::MSMINCHO);
		info_label_.GetStyle()->SetTextFont(*english_font);
		event_manager->AddGameObject(&info_label_);

		// Create animon and initialize the values we'll need to stick them into the dictionary
		Dictionary *dictionary = Game::GetInstance()->GetDictionary();
		event_manager->RegisterListener(Event::E_HIRAGANA_DRAWN, this);
		event_manager->RegisterListener(Event::E_GAME_EVENT, this);
		float y = 240.f;
		float width = (size.x - 50.f) / 5.f;
		for (size_t i = 0; i < hiraganas_.getSize(); i++) {
			sf::String h = hiraganas_[i];

			ProgressBar *progressbar = new ProgressBar((i + 0.45f) * width, 0, y + 128, refs::textures::objects::FOODBOWL, refs::textures::objects::SUSHI);
			progressbar->SetZ(-2);
			progressbars_.push_back(progressbar);
			event_manager->AddGameObject(progressbar);

			Animon *animon = new Animon((float)i * width, y, GameAssetManager::symbols_colors[asset_manager->GetHiraganaIndex(h[0])], h, progressbar);
			animon->LoadState(Animon::AnimonState::MEH);
			sf::Sprite sprite = animon->GetSprite();

			animon->SetZ(-1);
			animons_.push_back(animon);
			event_manager->AddGameObject(animon);
			event_manager->RegisterListener(Event::E_GAME_EVENT, this, animon);

			dictionary->AddWord(h);
		}

		Event load_event;
		load_event.type = Event::E_LOAD_STATE;
		event_manager->AddEvent(load_event);

		// Need to do this last
		Game::GetInstance()->GetTileListLabel()->SetString("");
		Game::GetInstance()->GetTileList()->Clear();
		Game::GetInstance()->AddUIElements();
	}

	void Feeding::UnregisterState(MachineState<GameState>* next_state) {
		GameAssetManager* manager = GameAssetManager::GetInstance();
		manager->ReturnTextures(this);

		Game::GetInstance()->RemoveUIElements();

		NewLevelState* state = (NewLevelState*) Game::GetInstance()->GetMachine()->GetState(GameState::NEW_LEVEL_LOAD);

		EventManager* event_manager = Game::GetInstance()->GetEventManager();
		event_manager->RemoveGameObject(&info_label_);
		event_manager->UnregisterListener(Event::E_GAME_EVENT, state);
		event_manager->UnregisterListener(Event::E_HIRAGANA_DRAWN, this);
		for (size_t i = 0; i < animons_.size(); i++) {
			Animon *animon = animons_[i];
			event_manager->RemoveGameObject(animon);
			event_manager->UnregisterListener(Event::E_GAME_EVENT, this, animon);
			delete animon;

			ProgressBar* bar = progressbars_[i];
			event_manager->RemoveGameObject(progressbars_[i]);
			delete bar;
		}

		animons_.clear();
		progressbars_.clear();

		Dictionary *dictionary = Game::GetInstance()->GetDictionary();
		dictionary->Clear();
	}

	GameState Feeding::Update(float elapsed_time) {
		bool all_happy = true;
		bool one_happy = false;
		GameAssetManager* manager = GameAssetManager::GetInstance();

		for (size_t i = 0; i < animons_.size(); i++) {
			ProgressBar *progressbar = progressbars_[i];
			float progress = progressbar->GetProgress();
			progress -= eat_rate_ * elapsed_time;
			progressbar->SetProgress(progress);

			Animon *animon = animons_[i];
			if (progress >= happy_threshold_) {
				animon->LoadState(Animon::AnimonState::HAPPY);
				one_happy = true;
			} else if (progress >= ok_threshold_) {
				animon->LoadState(Animon::AnimonState::MEH);
				all_happy = false;
			} else {
				animon->LoadState(Animon::AnimonState::ANGRY);
				all_happy = false;
			}
		}

		/* Reload */
		if (all_happy && game_state_ == GameState::FEEDING)
			game_state_ = GameState::NEW_LEVEL_LOAD;
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
			{
				Card *card = Game::GetInstance()->GetTileListLabel();
				if (event.message.isEmpty()) {
					card->Hide();
					card->GetStyle()->SetNormalColor(sf::Color::White);
				} else {
					card->Show();
					if (event.hiraganaEvent.accuracy >= CharacterTile::MIN_ACCURACY) {
						hiragana = event.message;
						card->GetStyle()->SetNormalColor(sf::Color::Green);
					} else {
						card->GetStyle()->SetNormalColor(sf::Color::Red);
					}
				}
			}
			break;
		case Event::E_GAME_EVENT:
			if (event.gameEvent == GameEvent::KEY_CLICK) {
				if (hiragana.getSize() > 0) {
					size_t i = 0;
					for (; i < animons_.size(); i++) {
						if (animons_[i] == event.source)
							break;
					}
					if (i == animons_.size())
						break;

					/* Check if we got the hiragana right. */
					if (event.message == hiragana) {
						ProgressBar *progressbar = progressbars_[i];
						progressbar->SetProgress(progressbar->GetProgress() + feed_increment_);
						animons_[i]->GetCry().play();
						event.gameEvent = GameEvent::ANIMON_RIGHT;
						Game::GetInstance()->GetEventManager()->AddEvent(event);
					}

					Game::GetInstance()->GetTileList()->Clear();
					hiragana.clear();
				}
			}
			break;
		}
	}

};