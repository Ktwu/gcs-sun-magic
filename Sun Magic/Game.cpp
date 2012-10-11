#include "stdafx.h"
#include "game.h"

#include <fstream>
#include <iostream>

#include "time.h"

namespace SunMagic {

Game::Game() {
	_gameState = Game::Playing;
	_mainWindow.Create(sf::VideoMode(1024, 768, 32), "Sun Magic!");
	
	// Load up list of unicode characters to use, then load it from the font
	std::vector<unsigned short> *chars = new std::vector<unsigned short>();
	// Ascii values
	for (unsigned short i = 0x20; i <= 0x7E; i++) {
			chars->push_back(i);
	}
	// Hiragana unicode values
	for (unsigned short i = 0x3041; i <= 0x3096; i++) {
			chars->push_back(i);
	}
	chars->shrink_to_fit();
	if (!_font.LoadFromFile("msmincho.ttc", 50, chars->data())) {
		std::cerr << "ERROR: Unable to load font msmincho.ttc." << std::endl;
	}
	delete chars;

	// Init zinnia
	CharacterTile::InitRecognizer("zinnia/models/hiragana.model");

	_tile = new CharacterTile(_mainWindow.GetWidth() / 2 - 150, _mainWindow.GetHeight() / 2 - 150, 300, 300);
	zinnia::Character *character = zinnia::Character::create();
	character->parse("(character (value ‚¢) (width 300) (height 300) (strokes ((56 63)(43 213)(67 259)(94 243)) ((213 66)(231 171)(208 217))))");
	_tile->SetGuideCharacter(character, sf::Unicode::Text(L"‚¢"));
	_tile->SetAnimationStroke(0);

	unsigned int width = _mainWindow.GetWidth();
	unsigned int y = _mainWindow.GetHeight() - 100;
	sf::String s = sf::String();
	s.SetFont(_font);
	s.SetColor(sf::Color(200, 200, 200));
	s.SetPosition(width / 10, y);
	_suggestedChars.push_back(s);

	s.SetX(width / 2);
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
	sf::Unicode::UTF32String str;
	str = sf::Unicode::Text("Current: ");
	str.append(_tile->GetUnicode());
	_suggestedChars[0].SetText(str);

	str = sf::Unicode::Text("Target: ");
	str.append(_tile->GetGuideUnicode());
	_suggestedChars[1].SetText(str);
}

void Game::HandleInput() {
	static const int MIN_STROKE_DISPLACEMENT_SQUARED = 10 * 10;

	static sf::Event currentEvent;
	static bool writing = false;
	static int last_x;
	static int last_y;

	while(_mainWindow.GetEvent(currentEvent)) {
		switch(_gameState) {
			case Game::Playing: {
				switch (currentEvent.Type) {

					case sf::Event::MouseButtonPressed:
						if (currentEvent.MouseButton.Button == sf::Mouse::Left) {
							writing = true;
							int stroke = _tile->NumStrokes();
							last_x = currentEvent.MouseButton.X;
							last_y = currentEvent.MouseButton.Y;
							_tile->AddStrokePoint(last_x, last_y);
							std::cout << "Start Stroke (" << std::dec << last_x << "," << last_y << ")" << std::endl;
						}
						break;

					case sf::Event::MouseMoved:
						if (writing) {							
							int x = currentEvent.MouseMove.X;
							int y = currentEvent.MouseMove.Y;
							// Only register strokes every if displacement is greater than MIN_STROKE_DISPLACEMENT_SQUARED
							if ((last_x - x) * (last_x - x) + (last_y - y) * (last_y - y) < MIN_STROKE_DISPLACEMENT_SQUARED)
								break;
							
							int stroke = _tile->NumStrokes() - 1;
							_tile->AddStrokePoint(x, y);
							last_x = x;
							last_y = y;
							//std::cout << "Add stroke segment " << _character->stroke_size(stroke) << std::endl;
						}
						break;

					case sf::Event::MouseButtonReleased:
						if (currentEvent.MouseButton.Button == sf::Mouse::Left) {
							if (!writing) break;
							writing = false;
							_tile->EndStroke();
							_tile->SetAnimationStroke(_tile->NumStrokes());
							std::cout << "End Stroke" << std::endl;

							UpdateText();
						}
						break;

					case sf::Event::KeyPressed:
						switch(currentEvent.Key.Code) {
							case sf::Key::C:
								std::cout << "Clear Strokes" << std::endl;
								writing = false;
								_tile->Clear();
								_tile->SetAnimationStroke(0);
								UpdateText();
								break;
							case sf::Key::U:
								std::cout << "Undo Stroke" << std::endl;
								if (writing) {
									writing = false;
									_tile->EndStroke();
								}
								_tile->UndoStroke();
								_tile->SetAnimationStroke(_tile->NumStrokes());
								UpdateText();
								break;
							case sf::Key::Escape:
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
	_mainWindow.Clear(sf::Color(0,0,0));

	_tile->Draw(&_mainWindow);

	// Draw suggested characters
	for (size_t i = 0; i < _suggestedChars.size(); i++) {
		_mainWindow.Draw(_suggestedChars[i]);
	}

	_mainWindow.Display();
}

void Game::Close() {
	_mainWindow.Close();
}

Game* Game::_game = new Game();

}