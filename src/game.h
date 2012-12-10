#pragma once

#include "stdafx.h"

#include "events/event.h"
#include "states/game_state.h"
#include "states/machine.h"
#include "ui/character_tilelist.h"
#include "ui/dictionary.h"
#include "ui/ui_element.h"
#include "ui/ui_group.h"

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
		Machine<GameState>* GetMachine();

		void AddUIElements();
		void RemoveUIElements();
		CharacterTileList* GetTileList();
		Dictionary* GetDictionary();
		UiElement* GetTileListLabel();

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

		/* UI elements shared by everyone */
		CharacterTileList* tilelist_;
		Dictionary* dict_;
		UiElement* listlabel_;
		
		EventManager *event_manager_;
		GameState game_state_;
		Machine<GameState> game_machine_;

		sf::RenderWindow main_window_;
		sf::Font font_;
	};
}