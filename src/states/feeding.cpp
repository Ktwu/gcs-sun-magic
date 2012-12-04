#include "stdafx.h"
#include "feeding.h"

#include "game.h"
#include "assets/gameasset_manager.h"
#include "events/event_manager.h"
#include "events/gameevent_manager.h"
#include "objects/animon.h"
#include "references/refs.h"
#include "tools/tools.h"

namespace sun_magic {

	Feeding::Feeding() :
		background_(),
		hiraganas_(),
		animons_(),
		foodbowls_(),
		game_state_()
	{
	}

	Feeding::~Feeding() {}

	void Feeding::SetHiraganas(std::vector<sf::String> hiraganas) {
		hiraganas_ = hiraganas;
	}

	void Feeding::RegisterState(MachineState<GameState>* previous_state) {
		game_state_ = FEEDING;

		GameAssetManager* manager = GameAssetManager::GetInstance();
		background_.setTexture(*manager->GetTexture(refs::textures::backgrounds::OFFICE));
		// The image might be a little too big, so scale it so it fits in the window
		tools::ScaleToWindowSize(background_);

		// Load our game's UI
		Game::GetInstance()->AddUIElements();
		sf::Vector2u size = Game::GetInstance()->GetWindow()->getSize();

		// Create animon objects
		EventManager* event_manager = Game::GetInstance()->GetEventManager();
		event_manager->RegisterListener(Event::E_HIRAGANA_DRAWN, this);
		for (size_t i = 0; i < hiraganas_.size(); i++) {
			sf::String h = hiraganas_[i];
			Animon *animon = new Animon((i + 0.5f) * size.x / (hiraganas_.size() + 1.f), 100.f, refs::textures::objects::NEKO, GameAssetManager::symbols_colors[manager->GetHiraganaIndex(h[0])], h);
			animons_.push_back(animon);
			event_manager->AddGameObject(animon);
			event_manager->RegisterListener(Event::E_GAME_EVENT, this, animon);
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
			if (event.message == hiragana) {
				Animon *animon = (Animon*)event.source;
			} else {
			}
			hiragana = "";
			break;
		}
	}

};