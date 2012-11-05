#pragma once

#include "stdafx.h"
#include "character_tile.h"
#include "game_states.h"
#include "machine.h"
#include "machine_states.h"
#include "sm_mouse.h"

namespace sun_magic {

	class Game {
	public:
		static Game* GetInstance() {
			if (Game::instance_ == NULL) {
				Game::instance_ = new Game();
			}
			return Game::instance_;
		}

		Game() {}
		~Game() {}

		void Init();
		void Run();
		void Destroy();

	private:
		static Game* instance_;

		void UpdateText();
		void HandleInput();
		void Update(float elapsed_seconds);
		void Draw();
		void Close();

		SMMouse mouse_;
		Machine<ref::MachineStates> game_machine_;
		ref::GameStates game_state_;
		sf::RenderWindow main_window_;
		sf::Font font_;
		std::vector<sf::Text> ui_strings_;
		EventManager event_manager_;
		CharacterTile* tile_;
	};
}