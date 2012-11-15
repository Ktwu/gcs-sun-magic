#include "stdafx.h"
#include "splash.h"

#include <Windows.h>

#include "game.h"
#include "assets/gameasset_manager.h"
#include "references/texture_refs.h"
#include "tools/tools.h"

namespace sun_magic {

	Splash::Splash() :
		background_(),
		text_("Loading"),
		text_cumulated_time_(),
		text_state_()
	{
		sf::Vector2u window_size = Game::GetInstance()->GetWindow()->getSize();
		sf::FloatRect text_rect = text_.getLocalBounds();
		float scale = 3.f;
		text_.setScale(scale, scale);
		text_.setPosition((sf::Vector2f(window_size) - scale * sf::Vector2f(text_rect.width, text_rect.height)) * 0.5f);
	}

	Splash::~Splash() {}
	
	DWORD WINAPI MyThreadFunction( LPVOID lpParam ) 
	{
		Splash *splash = (Splash *)lpParam;
		splash->LoadAssets();
		return 0; 
	} 

	void Splash::RegisterState(MachineState<GameState>* previous_state) {
		game_state_ = LOADING;

		GameAssetManager* manager = GameAssetManager::GetInstance();
		background_.setTexture(*manager->GetTexture(textures::backgrounds::POSTER_AWAY));
		/* The image might be a little too big, so scale it so it fits in the window */
		tools::ScaleToWindowSize(background_);

		// Start a thread to load assets in the background
		CreateThread(NULL, 0, &MyThreadFunction, this, 0, 0);
	}

	void Splash::UnregisterState(MachineState<GameState>* next_state) {
		GameAssetManager* manager = GameAssetManager::GetInstance();
		manager->ReturnTexture(textures::backgrounds::POSTER_AWAY);
	}

	GameState Splash::Update(float elapsed_time) {
		text_cumulated_time_ += elapsed_time;
		if (text_cumulated_time_ > 0.3f) {
			text_cumulated_time_ -= 0.3f;
			text_state_ = (text_state_ + 1) % 4;
			switch (text_state_) {
			case 0:
				text_.setString("Loading");
				break;
			case 1:
				text_.setString("Loading.");
				break;
			case 2:
				text_.setString("Loading..");
				break;
			case 3:
				text_.setString("Loading...");
				break;
			}
		}

		return game_state_;
	}
	
	void Splash::PreDraw(sf::RenderTarget *target) {
		Game::GetInstance()->GetWindow()->draw(background_);
		Game::GetInstance()->GetWindow()->draw(text_);
	}

	void Splash::PostDraw(sf::RenderTarget *target) {
	}

	void Splash::LoadAssets() {
		/*GameAssetManager* manager = GameAssetManager::GetInstance();
		manager->GetTexture(texture_refs::tutorial::poster_close);
		Yield();
		manager->GetTexture(texture_refs::tutorial::poster_away);
		Yield();
		manager->GetTexture(texture_refs::tutorial::window);
		Yield();
		manager->GetTexture(texture_refs::tutorial::desk);
		Yield();
		manager->GetTexture(texture_refs::tutorial::door);
		Yield();*/

		game_state_ = MAIN_MENU;
	}

}