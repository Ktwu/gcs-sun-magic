#include "stdafx.h"
#include "playing.h"

#include "game.h"
#include "gameasset_manager.h"
#include "texture_refs.h"
#include "tools_images.h"

namespace sun_magic {

	Playing::Playing() :
		background_()
	{
	}

	Playing::~Playing() {}

	void Playing::RegisterState(MachineState<ref::GameState>* previousState) {
		game_state_ = ref::PLAYING;

		GameAssetManager* manager = GameAssetManager::GetInstance();
		background_.setTexture(*manager->GetTexture(texture_refs::tutorial::window));
		/* The image might be a little too big, so scale it so it fits in the window */
		tools::images::ScaleToWindowSize(background_);
	}

	void Playing::UnregisterState(MachineState<ref::GameState>* previousState) {
		GameAssetManager* manager = GameAssetManager::GetInstance();
		manager->ReturnTexture(texture_refs::tutorial::window);
	}

	ref::GameState Playing::Update(float elapsed_time) {
		return game_state_;
	}
	
	void Playing::PreDraw(sf::RenderTarget *target) {
		target->draw(background_);
	}

	void Playing::PostDraw(sf::RenderTarget *target) {
	}

};