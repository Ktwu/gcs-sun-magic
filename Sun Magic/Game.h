#pragma once

#include "stdafx.h"
#include <hash_map>
#include <iostream>
#include <fstream>

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

	float _strokeWidth;
	sf::Color _strokeColor;
	std::vector<sf::Shape> _strokes;

	sf::Font _font;
	std::hash_map<unsigned short, unsigned short>* jisToUnicodeMap;
	std::vector<sf::String> _suggestedChars;

	zinnia::Recognizer *_recognizer;
	zinnia::Character *_character;
};

}