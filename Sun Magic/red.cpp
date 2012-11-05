#include "stdafx.h"
#include "machine_state.h"
#include "red.h"
#include "sm_mouse.h"
#include "game.h"
#include "gameasset_manager.h"
#include "texture_refs.h"
#include "tools_images.h"

namespace sun_magic {

	RedState::RedState() {}

	RedState::~RedState() {}

	void RedState::RegisterState(MachineState<ref::MachineStates>* previousState) {
		GameAssetManager* manager = GameAssetManager::GetInstance();
		_background_.setTexture(*manager->GetTexture(texture_refs::tutorial::poster_close));

		/* The image might be a little too big, so scale it so it fits in the window */
		tools::images::ScaleToWindowSize(_background_);
	}

	ref::MachineStates RedState::Update() {
		Game::GetInstance()->main_window_.draw(_background_);
		return HandleInput();
		// So long as I don't return an actual state, this'll keep running.
	}

	void RedState::UnregisterState(MachineState<ref::MachineStates>* previousState) {
		GameAssetManager* manager = GameAssetManager::GetInstance();
		manager->ReturnTexture(texture_refs::tutorial::poster_close);
	}

	ref::MachineStates RedState::HandleInput() {
		Game* game = Game::GetInstance();
		if (game->mouse_.haveButtonPressEvent &&
			game->mouse_.buttonPressEvent.button == sf::Mouse::Left) {
			std::cout << "Mouse pressed!\n";
			return  ref::BLUE;
		}

		return ref::NONE;
	}
};