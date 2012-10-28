#include "stdafx.h"
#include "blue.h"
#include "game.h"
#include "machine.h"
#include "machine_state.h"
#include "red.h"
#include "sm_mouse.h"
#include "time.h"

#include <fstream>
#include <iostream>

namespace SunMagic {

	SMMouse Game::mouse;
	Machine<Game::GameState> Game::gameMachine;
	Game::GameState Game::gameState;
	sf::RenderWindow Game::mainWindow;
	sf::Font Game::font;
	std::vector<sf::Text> Game::uiStrings;
	CharacterTile* Game::tile;

	void Game::init() {
		gameState = Game::Playing;
		mainWindow.create(sf::VideoMode(1024, 768, 32), "Sun Magic!");

		gameMachine.init(this->s_Blue, new BlueState());
		gameMachine.addState(this->s_Red, new RedState());

		// Load font data
		if (!font.loadFromFile("msmincho.ttc")) {
			std::cerr << "ERROR: Unable to load font msmincho.ttc." << std::endl;
		}

		// Init zinnia data
		CharacterTile::InitRecognizer("zinnia/models/hiragana.model");

		// Some stuff could be moved into a class
		sf::Vector2u size = mainWindow.getSize();
		tile = new CharacterTile(size.x * 0.5f - 150, size.y * 0.5f - 150, 300, 300);
		zinnia::Character *character = zinnia::Character::create();
		character->parse("(character (value い) (width 300) (height 300) (strokes ((56 63)(43 213)(67 259)(94 243)) ((213 66)(231 171)(208 217))))");
		tile->SetTraceCharacter(character, sf::String(L"い"));
		tile->SetAnimationStroke(0);
	
		unsigned int width = size.x;
		unsigned int y = size.y - 100;
		sf::Text s = sf::Text();
		s.setFont(font);
		s.setColor(sf::Color(200, 200, 200));
		s.setPosition(width * 0.01f, (float)y);
		uiStrings.push_back(s);

		s.setPosition(width * 0.5f, (float)y);
		uiStrings.push_back(s);
		
		y += 50;
		s = sf::Text();
		s.setColor(sf::Color(200, 0, 0));
		s.setPosition(width * 0.01f, (float)y);
		uiStrings.push_back(s);

		s.setPosition(width * 0.5f, (float)y);
		uiStrings.push_back(s);

		updateText();
	}

	void Game::handleInput() {
		sf::Event event;
		mouse.reset();

		while (mainWindow.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				gameState = Game::Exiting;

			if (!mouse.haveButtonPressEvent) {
				mouse.haveButtonPressEvent = event.type == sf::Event::MouseButtonPressed;
				mouse.buttonPressEvent = event.mouseButton;
			}

			if (!mouse.haveButtonReleaseEvent) {
				mouse.haveButtonReleaseEvent = event.type == sf::Event::MouseButtonReleased;
				mouse.buttonReleaseEvent = event.mouseButton;
			}

			if (!mouse.haveMoveEvent) {
				mouse.haveMoveEvent = event.type == sf::Event::MouseMoved;
				mouse.moveEvent = event.mouseMove;
			}

			if (!mouse.haveWheelEvent) {
				mouse.haveWheelEvent = event.type == sf::Event::MouseWheelMoved;
				mouse.wheelEvent = event.mouseWheel;
			}
		}
	}

	void Game::run() {	
		clock_t lastClock = clock();
		while(gameState != Game::Exiting) {
			clock_t newClock = clock();
			float elapsedSeconds = (float)(newClock - lastClock) / CLOCKS_PER_SEC;
			lastClock = newClock;
		
			handleInput();
			update(elapsedSeconds);
			draw();
		}
	}

	void Game::destroy() {
		close();
	}

	void Game::updateText() {
		sf::String str;
		str = sf::String("Current: ");
		str += tile->GetUnicode();
		uiStrings[0].setString(str);

		str = sf::String("Target: ");
		str += tile->GetTraceUnicode();
		uiStrings[1].setString(str);

		size_t strokes = tile->NumStrokes();
		if (strokes > 0) {
			std::stringstream ss;
			float error = tile->GetStrokeError(strokes - 1);
			ss << "Error: " << error;
			uiStrings[2].setString(sf::String(ss.str()));

			for (int i = strokes - 2; i >= 0; i--) {
				error += tile->GetStrokeError(i);
			}
			
			ss.str("");
			ss << "Total Error: " << error;
			uiStrings[3].setString(sf::String(ss.str()));
		} else {
			uiStrings[2].setString("Error: ");
			uiStrings[3].setString("Total Error: ");
		}
	}


	void Game::update(float elapsedSeconds) {
		tile->Update(elapsedSeconds);
	}


	void Game::draw() {
		gameMachine.update();
		tile->Draw(&mainWindow);

		// Draw ui strings
		for (size_t i = 0; i < uiStrings.size(); i++) {
			mainWindow.draw(uiStrings[i]);
		}

		updateText();
		mainWindow.display();
	}

	void Game::close() {
		mainWindow.close();
	}

}