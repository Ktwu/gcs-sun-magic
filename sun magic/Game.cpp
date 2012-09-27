#include "stdafx.h"
#include "Game.h"

#include <fstream>
#include <iostream>

#include "time.h"

#include "encoding.h"

namespace SunMagic {

Game::Game() {
	_gameState = Game::Playing;
	_mainWindow.Create(sf::VideoMode(1024, 768, 32), "Sun Magic!");

	_strokeWidth = 7;
	_strokeColor = sf::Color(200, 200, 200);

	//convertJISModelToUnicodeModel("zinnia/models/handwriting-ja.model.txt",
	// "zinnia/models/handwriting-ja-unicode.model.txt", "jis_to_unicode_map.txt");
	jisToUnicodeMap = GetJISToUnicodeMap("jis_to_unicode_map.txt");
	
	std::vector<sf::Uint16> *chars = GetUnicodeList("japanese_unicode.txt");
	if (!_font.LoadFromFile("msmincho.ttc", 50, chars->data())) {
		std::cerr << "ERROR: Unable to load font msmincho.ttc." << std::endl;
	}
	delete chars;

	// Init zinnia
	_recognizer = zinnia::Recognizer::create();
	if (!_recognizer->open("zinnia/models/handwriting-ja.model")) {
		std::cerr << "ERROR: " << _recognizer->what() << std::endl;
		return;
	}

	_character = zinnia::Character::create();
	_character->clear();
	_character->set_width(1024);
	_character->set_height(768);
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

void AddNewStroke() {

}

void Game::HandleInput() {
	static const int MIN_STROKE_DISPLACEMENT_SQUARED = 20 * 20;

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
							std::cout << "Start Stroke" << std::endl;
							writing = true;
							int stroke = _character->strokes_size();
							last_x = currentEvent.MouseButton.X;
							last_y = currentEvent.MouseButton.Y;
							_character->add(stroke, last_x, last_y);
						}
						break;

					case sf::Event::MouseMoved:
						if (writing) {							
							int x = currentEvent.MouseMove.X;
							int y = currentEvent.MouseMove.Y;
							// Only register strokes every if displacement is greater than MIN_STROKE_DISPLACEMENT_SQUARED
							if ((last_x - x) * (last_x - x) + (last_y - y) * (last_y - y) < MIN_STROKE_DISPLACEMENT_SQUARED)
								break;
							
							int stroke = _character->strokes_size() - 1;
							_character->add(stroke, x, y);
							_strokes.push_back(sf::Shape::Line((float)last_x, (float)last_y, (float)x, (float)y, _strokeWidth, _strokeColor));
							last_x = x;
							last_y = y;
							//std::cout << "Add stroke segment " << _character->stroke_size(stroke) << std::endl;
						}
						break;

					case sf::Event::MouseButtonReleased:
						if (currentEvent.MouseButton.Button == sf::Mouse::Left) {
							if (!writing) break;
							writing = false;
							std::cout << "End Stroke" << std::endl;

							// Reclassify
							zinnia::Result *result = _recognizer->classify(*_character, 5);
							if (result) {
								_suggestedChars.clear();
								int spacing = _mainWindow.GetWidth() / (result->size() + 1);
								for (size_t i = 0; i < result->size(); ++i) {
									std::hash_map<unsigned short, unsigned short>::iterator it;
									std::vector<sf::Uint16> unicode;
									const char *c = result->value(i);
									int len = (strlen(c) + 1) / 2;
									unsigned char a[] = { c[0], c[1], c[2], c[3], 0};

									std::cout << c << " (";
									for (int j = 0; j < len; j++) {
										// The byte order is little endian
										unsigned char d[] = { (2*j+1 == len) ? 0 : c[2*j+1], c[2*j] };
										unsigned short *jis = reinterpret_cast<unsigned short*>(d);
										std::cout << std::hex << *jis;
										if (j < len - 1)
											std::cout << ",";
										it = jisToUnicodeMap->find(*jis);
										if (it != jisToUnicodeMap->end())
											unicode.push_back(it->second);
									}
									std::cout << ")->(";
									for (int j = 0; j < unicode.size(); j++) {
										std::cout << std::hex << unicode[j];
										if (j < unicode.size() - 1)
											std::cout << ",";
									}
									std::cout << ")\t" << result->score(i) << std::endl;
									
									unicode.push_back(0);
									unicode.shrink_to_fit();

									sf::String s = sf::String(unicode.data());
									s.SetFont(_font);
									s.SetColor(_strokeColor);
									s.SetPosition((float)(i + 1) * spacing , 700);
									_suggestedChars.push_back(s);
								}
							} else {
								std::cerr << "ERROR: " << _recognizer->what() << std::endl;
								return;
							}
							delete result;
						}
						break;

					case sf::Event::KeyPressed:
						switch(currentEvent.Key.Code) {
							case sf::Key::C:
								std::cout << "Clear Strokes" << std::endl;
								writing = false;
								_character->clear();
								_strokes.clear();
								_suggestedChars.clear();
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
}


void Game::Draw() {
	_mainWindow.Clear(sf::Color(0,0,0));

	// Draw strokes of current character
	for (size_t i = 0; i < _strokes.size(); i++) {
		_mainWindow.Draw(_strokes[i]);
	}

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