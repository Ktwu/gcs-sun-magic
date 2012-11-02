#include "stdafx.h"
#include "blue.h"
#include "game.h"
#include "machine_state.h"
#include "gameasset_manager.h"
#include "sm_mouse.h"
#include "texture_refs.h"
#include "tools_images.h"

namespace sun_magic {

	BlueState::BlueState() {}

	BlueState::~BlueState() {}

	void BlueState::RegisterState(MachineState<ref::MachineStates>* previousState) {
		GameAssetManager* manager = GameAssetManager::GetInstance();
		_background_.setTexture(*manager->GetTexture(texture_refs::tutorial::poster_away));

		/* The image might be a little too big, so scale it so it fits in the window */
		tools::images::ScaleToWindowSize(_background_);
	}

	ref::MachineStates BlueState::Update() {
		Game::GetInstance()->main_window_.draw(_background_);
		return HandleInput();
		// So long as I don't return an actual state, this'll keep running.
	}

	void BlueState::UnregisterState(MachineState<ref::MachineStates>* previousState) {
		GameAssetManager* manager = GameAssetManager::GetInstance();
		manager->ReturnTexture(texture_refs::tutorial::poster_away);
		//manager->CleanUnusedTextures();
	}

	ref::MachineStates BlueState::HandleInput() {
		Game* game = Game::GetInstance();
		if (game->mouse_.haveButtonPressEvent &&
			game->mouse_.buttonPressEvent.button == sf::Mouse::Left) {
			std::cout << "Mouse pressed!\n";
			return  ref::RED;
		}

		return ref::NONE;
	}
};