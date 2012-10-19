#pragma once

#include "stdafx.h"
#include "character_tile.h"

namespace SunMagic {

class Game {

public:
	static Game* GetInstance() {
		return Game::_game;
	}
	void Run();

private:
	Game();
	~Game();
	void UpdateText();
	void HandleInput();
	void Update(float elapsedSeconds);
	void Draw();
	void Close();

	enum GameState {
		ShowingSplash,
		Paused, 
		ShowingMenu,
		Playing,
		Exiting
	};

	static Game* _game;
  
	GameState _gameState;
	sf::RenderWindow _mainWindow;

	sf::Font _font;
	std::vector<sf::Text> _suggestedChars;

	CharacterTile *_tile;
};

}