﻿#include "stdafx.h"
#include "blue.h"
#include "game.h"
#include "game_states.h"
#include "machine.h"
#include "machine_state.h"
#include "red.h"
#include "sm_mouse.h"
#include "time.h"

#include <fstream>
#include <iostream>

namespace sun_magic {

	Game* Game::instance_ = NULL;

	void Game::Init() {
		game_state_ = ref::PLAYING;
		main_window_.create(sf::VideoMode(1024, 768, 32), "Sun Magic!");

		game_machine_.Init(ref::BLUE, new BlueState());
		game_machine_.AddState(ref::RED, new RedState());

		// Load font data
		if (!font_.loadFromFile("msmincho.ttc")) {
			std::cerr << "ERROR: Unable to load font msmincho.ttc." << std::endl;
		}

		// Init zinnia data
		CharacterTile::InitRecognizer("zinnia/models/hiragana.model");

		// Some stuff could be moved into a class
		sf::Vector2u size = main_window_.getSize();
		tile_ = new CharacterTile(size.x * 0.5f - 150, size.y * 0.5f - 150, 300, 300);
		zinnia::Character *character = zinnia::Character::create();
		character->parse("(character (value い) (width 300) (height 300) (strokes ((56 63)(43 213)(67 259)(94 243)) ((213 66)(231 171)(208 217))))");
		tile_->SetTraceCharacter(character, sf::String(L"い"));
		tile_->SetAnimationStroke(0);
	
		unsigned int width = size.x;
		unsigned int y = size.y - 100;
		sf::Text s = sf::Text();
		s.setFont(font_);
		s.setColor(sf::Color(200, 200, 200));
		s.setPosition(width * 0.01f, (float)y);
		ui_strings_.push_back(s);

		s.setPosition(width * 0.5f, (float)y);
		ui_strings_.push_back(s);
		
		y += 50;
		s = sf::Text();
		s.setColor(sf::Color(200, 0, 0));
		s.setPosition(width * 0.01f, (float)y);
		ui_strings_.push_back(s);

		s.setPosition(width * 0.5f, (float)y);
		ui_strings_.push_back(s);

		UpdateText();
	}

	void Game::HandleInput() {
		sf::Event event;
		mouse_.reset();

		while (main_window_.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				game_state_ = ref::EXITING;

			if (!mouse_.haveButtonPressEvent) {
				mouse_.haveButtonPressEvent = event.type == sf::Event::MouseButtonPressed;
				mouse_.buttonPressEvent = event.mouseButton;
			}

			if (!mouse_.haveButtonReleaseEvent) {
				mouse_.haveButtonReleaseEvent = event.type == sf::Event::MouseButtonReleased;
				mouse_.buttonReleaseEvent = event.mouseButton;
			}

			if (!mouse_.haveMoveEvent) {
				mouse_.haveMoveEvent = event.type == sf::Event::MouseMoved;
				mouse_.moveEvent = event.mouseMove;
			}

			if (!mouse_.haveWheelEvent) {
				mouse_.haveWheelEvent = event.type == sf::Event::MouseWheelMoved;
				mouse_.wheelEvent = event.mouseWheel;
			}
		}
	}

	void Game::Run() {	
		clock_t last_clock = clock();
		while(game_state_ != ref::EXITING) {
			clock_t new_clock = clock();
			float elapsed_seconds = (float)(new_clock - last_clock) / CLOCKS_PER_SEC;
			last_clock = new_clock;
		
			HandleInput();
			Update(elapsed_seconds);
			Draw();
		}
	}

	void Game::Destroy() {
		Close();
	}

	void Game::UpdateText() {
		sf::String str;
		str = sf::String("Current: ");
		str += tile_->GetUnicode();
		ui_strings_[0].setString(str);

		str = sf::String("Target: ");
		str += tile_->GetTraceUnicode();
		ui_strings_[1].setString(str);

		size_t strokes = tile_->NumStrokes();
		if (strokes > 0) {
			std::stringstream ss;
			float error = tile_->GetStrokeError(strokes - 1);
			ss << "Error: " << error;
			ui_strings_[2].setString(sf::String(ss.str()));

			for (int i = strokes - 2; i >= 0; i--) {
				error += tile_->GetStrokeError(i);
			}
			
			ss.str("");
			ss << "Total Error: " << error;
			ui_strings_[3].setString(sf::String(ss.str()));
		} else {
			ui_strings_[2].setString("Error: ");
			ui_strings_[3].setString("Total Error: ");
		}
	}

	void Game::Update(float elapsed_seconds) {
		tile_->Update(elapsed_seconds);
	}

	void Game::Draw() {
		game_machine_.Update();
		tile_->Draw(&main_window_);

		// Draw ui strings
		for (size_t i = 0; i < ui_strings_.size(); i++) {
			main_window_.draw(ui_strings_[i]);
		}

		UpdateText();
		main_window_.display();
	}

	void Game::Close() {
		main_window_.close();
	}

}