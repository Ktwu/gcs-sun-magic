#pragma once

#include "stdafx.h"
#include "character_tile.h"
#include "event_manager.h"
#include "game_states.h"
#include "machine.h"
#include "machine_states.h"

namespace sun_magic {

	class Game : public EventListener {
	public:
		Game();
		~Game();

		static Game* GetInstance() {
			if (Game::instance_ == NULL) {
				Game::instance_ = new Game();
			}
			return Game::instance_;
		}

		sf::RenderWindow* GetWindow();
		EventManager* GetEventManager();

		void Init();
		void Run();
		void Destroy();
		void ProcessEvent(Event *event);

	private:

		void UpdateText();
		void HandleInput();
		void Update(float elapsed_seconds);
		void Draw();
		void Close();

		static Game* instance_;

		Machine<ref::MachineStates> game_machine_;
		ref::GameStates game_state_;
		EventManager event_manager_;
		sf::RenderWindow main_window_;
		sf::Font font_;
		std::vector<sf::Text> ui_strings_;
		CharacterTile* tile_;
	};
}