#pragma once

#include "stdafx.h"
#include "character_tile.h"
#include "machine.h"
#include "sm_mouse.h"

namespace SunMagic {

	class Game {
	public:
		Game() {}
		~Game() {}

		void init();
		void handleInput();
		void run();
		void destroy();

		static enum GameState {
			ShowingSplash,
			Paused, 
			ShowingMenu,
			Playing,
			Exiting,
			s_None,
			s_Red,
			s_Blue
		};

		static SMMouse mouse;
		static Machine<GameState> gameMachine;
		static GameState gameState;
		static sf::RenderWindow mainWindow;
		static sf::Font font;
		static std::vector<sf::Text> suggestedChars;
		static CharacterTile* tile;

	private:
		void updateText();
		void update(float elapsedSeconds);
		void draw();
		void close();
	};
}