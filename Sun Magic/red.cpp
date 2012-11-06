#include "stdafx.h"
#include "red.h"

#include "game.h"
#include "gameasset_manager.h"
#include "machine_state.h"
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

		change_ = false;
		RedState *self = static_cast<RedState*>(this);
		Game::GetInstance()->GetEventManager()->RegisterListener(Event::E_MOUSE_RELEASED, this);
	}

	void RedState::UnregisterState(MachineState<ref::MachineStates>* previousState) {
		GameAssetManager* manager = GameAssetManager::GetInstance();
		manager->ReturnTexture(texture_refs::tutorial::poster_close);
		
		Game::GetInstance()->GetEventManager()->UnregisterListener(Event::E_MOUSE_RELEASED, this);
	}

	ref::MachineStates RedState::Update(float elapsed_time) {
		Game::GetInstance()->GetWindow()->draw(_background_);

		if (change_) {
			return ref::BLUE;
		}
		return ref::NONE;
	}

	void RedState::ProcessEvent(Event *event) {
		switch(event->type) {
		case Event::E_MOUSE_RELEASED:
			change_ = true;
			break;
		}
	}

};