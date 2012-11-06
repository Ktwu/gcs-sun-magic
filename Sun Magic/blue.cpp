#include "stdafx.h"
#include "blue.h"

#include "game.h"
#include "machine_state.h"
#include "gameasset_manager.h"
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

		change_ = false;
		BlueState *self = static_cast<BlueState*>(this);
		Game::GetInstance()->GetEventManager()->RegisterListener(Event::E_MOUSE_RELEASED, this);
	}

	void BlueState::UnregisterState(MachineState<ref::MachineStates>* previousState) {
		GameAssetManager* manager = GameAssetManager::GetInstance();
		manager->ReturnTexture(texture_refs::tutorial::poster_close);
		
		Game::GetInstance()->GetEventManager()->UnregisterListener(Event::E_MOUSE_RELEASED, this);
	}

	ref::MachineStates BlueState::Update() {
		Game::GetInstance()->GetWindow()->draw(_background_);

		if (change_) {
			return ref::RED;
		}
		return ref::NONE;
	}

	void BlueState::ProcessEvent(Event *event) {
		switch(event->type) {
		case Event::E_MOUSE_RELEASED:
			change_ = true;
			break;
		}
	}

};