#pragma once

#include "stdafx.h"
#include "events/event.h"
#include "states/game_state.h"
#include "states/machine.h"

namespace sun_magic {
	
	class CharacterTile;
	class Dictionary;
	class EventManager;

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
		Dictionary* GetDictionary();

		void Init();
		void Run();
		void Destroy();
		void ProcessEvent(Event event);

	private:

		void UpdateText();
		void HandleInput();
		void Update(float elapsed_seconds);
		void Draw();
		void Close();

		static Game* instance_;

		CharacterTile* tile_;
		Dictionary* dict_;
		EventManager *event_manager_;
		GameState game_state_;
		Machine<GameState> game_machine_;

		sf::RenderWindow main_window_;
		sf::Font font_;

		std::vector<sf::Text> ui_strings_;
	};
}