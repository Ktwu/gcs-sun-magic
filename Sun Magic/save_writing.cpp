#pragma once

#include "stdafx.h"
#include "character_tile.h"
#include "game.h"
#include "save_writing.h";
#include "machine_state.h"
#include "machine_states.h"

namespace sun_magic {

	SaveWritingState::SaveWritingState() {
		sf::Vector2u size = Game::GetInstance()->main_window_.getSize();
		_tile_ = new CharacterTile(size.x * 0.5f - 150, size.y * 0.5f - 150, 300, 300);

		//zinnia::Character *character = zinnia::Character::create();
		//character->parse("(character (value い) (width 300) (height 300) (strokes ((56 63)(43 213)(67 259)(94 243)) ((213 66)(231 171)(208 217))))");
		//_tile_->SetTraceCharacter(NULL, sf::String(L"い"));
	
		sf::Font& font = Game::GetInstance()->font_;
		unsigned int width = size.x;
		unsigned int y = size.y - 200;

		/* Set up the strings to display in the scene */
		_prompt_.setFont(font);
		_prompt_.setColor(sf::Color(200, 200, 200));
		_prompt_.setPosition(width * 0.30f, (float)y);
		_current_.setFont(font);
		_current_.setColor(sf::Color(200, 200, 200));
		_current_.setPosition(width * 0.30f, (float)y + 50);
	}

	SaveWritingState::~SaveWritingState() {
		delete _tile_;
	}
	
	void SaveWritingState::RegisterState(MachineState<ref::MachineStates>* previous_state) {
	}

	ref::MachineStates SaveWritingState::Update() {
		sf::RenderWindow* window = &Game::GetInstance()->main_window_;
		window->clear(sf::Color::Black);

		/* Draw and update the tile */
		_tile_->Update();
		window->draw(*(sf::Drawable*)_tile_);

		/* Draw UI strings */
		UpdateText();
		window->draw(_prompt_);
		window->draw(_current_);

		return ref::MachineStates::NONE;
	}

	void SaveWritingState::UnregisterState(MachineState<ref::MachineStates>* previous_state) {
	}

	void SaveWritingState::UpdateText() {
		_current_.setString("Current: " + _tile_->GetUnicode());
		_prompt_.setString("Please Draw: " + _tile_->GetTraceUnicode());

		char buff[256];
		_tile_->GetCharacter()->toString(buff, 256);
		std::cout << buff << "\n";
		/*size_t strokes = _tile_->NumStrokes();
		if (strokes > 0) {
			std::stringstream ss;
			float error = _tile_->GetStrokeError(strokes - 1);
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
		}*/
	}

	ref::MachineStates SaveWritingState::HandleInput() {
		return ref::MachineStates::NONE;
	}

}