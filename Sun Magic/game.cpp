#include "stdafx.h"
#include "game.h"

#include <fstream>
#include <iostream>

#include "time.h"

namespace SunMagic {

Game::Game() {
	_gameState = Game::Playing;
	_mainWindow.create(sf::VideoMode(1024, 768, 32), "Sun Magic!");

	// Load font
	if (!_font.loadFromFile("msmincho.ttc")) {
		std::cerr << "ERROR: Unable to load font msmincho.ttc." << std::endl;
	}

	// Init zinnia
	CharacterTile::InitRecognizer("zinnia/models/hiragana.model");

	sf::Vector2u size = _mainWindow.getSize();
	_tile = new CharacterTile(size.x * 0.5f - 150, size.y * 0.5f - 150, 300, 300);
	zinnia::Character *character = zinnia::Character::create();
	character->parse("(character (value い) (width 300) (height 300) (strokes ((56 63)(43 213)(67 259)(94 243)) ((213 66)(231 171)(208 217))))");
	_tile->SetTraceCharacter(character, sf::String(L"い"));
	_tile->SetAnimationStroke(0);
	
	unsigned int width = size.x;
	unsigned int y = size.y - 100;
	sf::Text s = sf::Text();
	s.setFont(_font);
	s.setColor(sf::Color(200, 200, 200));
	s.setPosition(width * 0.1f, (float)y);
	_suggestedChars.push_back(s);

	s.setPosition(width * 0.5f, (float)y);
	_suggestedChars.push_back(s);

	UpdateText();
}

Game::~Game() {
}

void Game::Run() {	
	clock_t lastClock = clock();
	while(_gameState != Game::Exiting) {
		clock_t newClock = clock();
		float elapsedSeconds = (float)(newClock - lastClock) / CLOCKS_PER_SEC;
		lastClock = newClock;
		
		HandleInput();
		Update(elapsedSeconds);
		Draw();
	}

	Close();
}

void Game::UpdateText() {
	sf::String str;
	str = sf::String("Current: ");
	str += _tile->GetUnicode();
	_suggestedChars[0].setString(str);

	str = sf::String("Target: ");
	str += _tile->GetTraceUnicode();
	_suggestedChars[1].setString(str);
}

void Game::HandleInput() {
	static const int MIN_STROKE_DISPLACEMENT_SQUARED = 8 * 8;

	static sf::Event currentEvent;
	static bool writing = false;
	static float last_x;
	static float last_y;

	while(_mainWindow.pollEvent(currentEvent)) {
		switch(_gameState) {
			case Game::Playing: {
				switch (currentEvent.type) {

					case sf::Event::MouseButtonPressed:
						if (currentEvent.mouseButton.button == sf::Mouse::Left) {
							writing = true;
							int stroke = _tile->NumStrokes();
							last_x = (float)currentEvent.mouseButton.x;
							last_y = (float)currentEvent.mouseButton.y;
							_tile->AddStrokePoint(sf::Vector2f(last_x, last_y));
							std::cout << "Start Stroke (" << std::dec << last_x << "," << last_y << ")" << std::endl;
						}
						break;

					case sf::Event::MouseMoved:
						if (writing) {							
							float x = (float)currentEvent.mouseMove.x;
							float y = (float)currentEvent.mouseMove.y;
							// Only register strokes every if displacement is greater than MIN_STROKE_DISPLACEMENT_SQUARED
							if ((last_x - x) * (last_x - x) + (last_y - y) * (last_y - y) < MIN_STROKE_DISPLACEMENT_SQUARED)
								break;
							
							int stroke = _tile->NumStrokes() - 1;
							_tile->AddStrokePoint(sf::Vector2f(x, y));
							last_x = x;
							last_y = y;
							//std::cout << "Add stroke segment " << _character->stroke_size(stroke) << std::endl;
						}
						break;

					case sf::Event::MouseButtonReleased:
						if (currentEvent.mouseButton.button == sf::Mouse::Left) {
							if (!writing) break;
							writing = false;
							_tile->EndStroke();
							_tile->SetAnimationStroke(_tile->NumStrokes());
							std::cout << "End Stroke" << std::endl;

							UpdateText();
						}
						break;

					case sf::Event::KeyPressed:
						switch(currentEvent.key.code) {
							case sf::Keyboard::C:
								std::cout << "Clear Strokes" << std::endl;
								writing = false;
								_tile->Clear();
								_tile->SetAnimationStroke(0);
								UpdateText();
								break;
							case sf::Keyboard::U:
								std::cout << "Undo Stroke" << std::endl;
								if (writing) {
									writing = false;
									_tile->EndStroke();
								}
								_tile->UndoStroke();
								_tile->SetAnimationStroke(_tile->NumStrokes());
								UpdateText();
								break;
							case sf::Keyboard::Escape:
								_gameState = Game::Exiting;
								break;
						}
						break;

					case sf::Event::Closed:
						_gameState = Game::Exiting;
						break;
				}
				break;
			}
		}
	}
}

void Game::Update(float elapsedSeconds) {
	_tile->Update(elapsedSeconds);
}


void Game::Draw() {
	_mainWindow.clear(sf::Color(0,0,0));

	_tile->Draw(&_mainWindow);

	// Draw suggested characters
	for (size_t i = 0; i < _suggestedChars.size(); i++) {
		_mainWindow.draw(_suggestedChars[i]);
	}

	_mainWindow.display();
}

void Game::Close() {
	_mainWindow.close();
}

Game* Game::_game = new Game();

}