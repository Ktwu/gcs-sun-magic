#include "stdafx.h"
#include "playing.h"

#include "game.h"
#include "assets/gameasset_manager.h"
#include "events/event_manager.h"
#include "references/texture_refs.h"
#include "tools/tools.h"

namespace sun_magic {

	Playing::Playing() :
		background_(),
		tilelist_(0, 0, 1000, 200, 5)
	{
	}

	Playing::~Playing() {}

	void Playing::RegisterState(MachineState<GameState>* previous_state) {
		game_state_ = PLAYING;

		GameAssetManager* manager = GameAssetManager::GetInstance();
		background_.setTexture(*manager->GetTexture(textures::backgrounds::WINDOW));
		/* The image might be a little too big, so scale it so it fits in the window */
		tools::ScaleToWindowSize(background_);

		EventManager* event_manager = Game::GetInstance()->GetEventManager();
		event_manager->AddGameObject(&tilelist_);
		event_manager->RegisterListener(Event::E_HIRAGANA_DRAWN, this, &tilelist_);
	}

	void Playing::UnregisterState(MachineState<GameState>* next_state) {
		GameAssetManager* manager = GameAssetManager::GetInstance();
		manager->ReturnTexture(textures::backgrounds::WINDOW);

		EventManager* event_manager = Game::GetInstance()->GetEventManager();
		event_manager->RemoveGameObject(&tilelist_);
		event_manager->UnregisterListener(Event::E_HIRAGANA_DRAWN, this, &tilelist_);
	}

	GameState Playing::Update(float elapsed_time) {
		tilelist_.Update(elapsed_time);
		return game_state_;
	}
	
	void Playing::PreDraw(sf::RenderTarget *target) {
		target->draw(background_);

		tilelist_.Draw(target);
	}

	void Playing::PostDraw(sf::RenderTarget *target) { }

	void Playing::ProcessEvent(Event event) {
		switch (event.type) {
		case Event::E_HIRAGANA_DRAWN:
			std::wcout << "Got string " << event.message.toWideString() << "\n";
		}
	}

};