#include "stdafx.h"
#include "playing.h"

#include "game.h"
#include "assets/gameasset_manager.h"
#include "events/event_manager.h"
#include "events/gameevent_manager.h"
#include "references/event_refs.h"
#include "references/texture_refs.h"
#include "tools/tools.h"

namespace sun_magic {

	Playing::Playing() :
		background_()
	{
		GameEventManager* manager = GameEventManager::GetInstance();
		std::set<KeyObject*>& keys = manager->GetKeyObjectsFor(this);

		keys.insert(new KeyObject(0, 150, textures::objects::NEKO, sf::Color::White, L"ねこ", event_callbacks::Cat));
		keys.insert(new KeyObject(200, 150, textures::objects::STUPID_LOVE, sf::Color::White, L"あい", event_callbacks::Love, true, false));
	}

	Playing::~Playing() {}

	void Playing::RegisterState(MachineState<GameState>* previous_state) {
		game_state_ = PLAYING;

		GameAssetManager* manager = GameAssetManager::GetInstance();
		background_.setTexture(*manager->GetTexture(textures::backgrounds::WINDOW));
		// The image might be a little too big, so scale it so it fits in the window
		tools::ScaleToWindowSize(background_);

		// Load our game's UI
		Game::GetInstance()->AddUIElements();

		EventManager* event_manager = Game::GetInstance()->GetEventManager();
		GameEventManager* game_event_manager = GameEventManager::GetInstance();
		std::set<KeyObject*> keys = game_event_manager->GetKeyObjectsFor(this);
		for (std::set<KeyObject*>::iterator i = keys.begin(); i != keys.end(); ++i)
			event_manager->AddGameObject(*i);

		Event load_event;
		load_event.type = Event::E_LOAD_STATE;
		event_manager->AddEvent(load_event);
	}

	void Playing::UnregisterState(MachineState<GameState>* next_state) {
		GameAssetManager* manager = GameAssetManager::GetInstance();
		manager->ReturnTexture(textures::backgrounds::WINDOW);

		Game::GetInstance()->RemoveUIElements();

		EventManager* event_manager = Game::GetInstance()->GetEventManager();
		GameEventManager* game_event_manager = GameEventManager::GetInstance();
		std::set<KeyObject*>& keys = game_event_manager->GetKeyObjectsFor(this);
		for (std::set<KeyObject*>::iterator i = keys.begin(); i != keys.end(); ++i)
			event_manager->RemoveGameObject(*i);
	}

	GameState Playing::Update(float elapsed_time) {
		return game_state_;
	}
	
	void Playing::PreDraw(sf::RenderTarget *target) {
		target->draw(background_);
	}

	void Playing::PostDraw(sf::RenderTarget *target) {
	}

	void Playing::ProcessEvent(Event event) {
	}

};