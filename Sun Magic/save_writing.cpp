#pragma once

#include "stdafx.h"
#include "character_tile.h"
#include "events.h"
#include "event_manager.h"
#include "game.h"
#include "save_writing.h";
#include "machine_state.h"
#include "machine_states.h"

namespace sun_magic {

	SaveWritingState::SaveWritingState() {
		sf::Vector2u size = Game::GetInstance()->GetWindow()->getSize();
		_tile_ = new CharacterTile(size.x * 0.5f - 150, size.y * 0.5f - 150, 300, 300);
		Game::GetInstance()->GetEventManager()->AddGameObject(_tile_);

		_trace_output_.open("traced_hiragana.txt");
		zinnia::Character *character = zinnia::Character::create();
		character->parse("(character (value )(width 300)(height 300)(strokes ((178 46)(81 148)(81 156)(176 243)))");
		//character->parse("(character (value い) (width 300) (height 300) (strokes ((56 63)(43 213)(67 259)(94 243)) ((213 66)(231 171)(208 217))))");
		_tile_->SetTraceCharacter(character, sf::String(L"い"));
		_tile_->SetAnimationSpeed(200.0f);
		_tile_->SetAnimationStroke(0);

		target_index = 0;
		target_hiragana = _tile_->UTF8ToUTF32(CharacterTile::GetRecognizer()->value(target_index));
		_have_trace_ = false;
	
		if (!_font_.loadFromFile("msmincho.ttc")) {
			std::cerr << "ERROR: Unable to load font msmincho.ttc." << std::endl;
		}
		unsigned int width = size.x;
		unsigned int y = size.y - 200;

		/* Set up the strings to display in the scene */
		_prompt_.setFont(_font_);
		_prompt_.setColor(sf::Color(200, 200, 200));
		_prompt_.setPosition(width * 0.30f, (float)y);
		_current_.setFont(_font_);
		_current_.setColor(sf::Color(200, 200, 200));
		_current_.setPosition(width * 0.30f, (float)y + 50);
		_save_.setFont(_font_);
		_save_.setColor(sf::Color(200, 200, 200));
		_save_.setPosition(width * 0.30f, (float)y + 100);
		_save_.setString("Press 'S' to save");
	}

	SaveWritingState::~SaveWritingState() {
		_trace_output_.close();
		delete _tile_;
	}
	
	void SaveWritingState::RegisterState(MachineState<ref::MachineStates>* previous_state) {
		_tile_->Register();
		Game::GetInstance()->GetEventManager()->RegisterListener(Event::E_KEY_RELEASED, this);
	}

	ref::MachineStates SaveWritingState::Update(float elapsed_time) {
		sf::RenderWindow* window = Game::GetInstance()->GetWindow();
		window->clear(sf::Color::Black);

		/* Draw and update the tile */
		_tile_->Update(elapsed_time);
		_tile_->Draw(window);

		/* Draw UI strings */
		UpdateText();
		window->draw(_prompt_);
		window->draw(_current_);

		if (_have_trace_)
			window->draw(_save_);

		return ref::MachineStates::NONE;
	}

	void SaveWritingState::UnregisterState(MachineState<ref::MachineStates>* previous_state) {
		_tile_->Unregister();
		Game::GetInstance()->GetEventManager()->UnregisterListener(Event::E_KEY_RELEASED, this);
	}

	void SaveWritingState::ProcessEvent(Event *event) {
		switch (event->type) {
		case Event::E_KEY_RELEASED:
			{
				KeyEvent *key_event = (KeyEvent*)event;
				switch (key_event->key) {

				case Keyboard::Space:
					_tile_->Clear();
					break;

				case Keyboard::S:
					if (!_have_trace_)
						break;

					// Switch to the next character to draw
					target_index = (target_index + 1) % CharacterTile::GetRecognizer()->size();
					std::cout << CharacterTile::GetRecognizer()->size() << "\n";
					target_hiragana = _tile_->UTF8ToUTF32(CharacterTile::GetRecognizer()->value(target_index));

					// Save our output
					_tile_->GetCharacter()->toString(buff, size);
					_trace_output_ << buff << "\n";
					std::cout << buff << "\n";

					// Clear things up
					_tile_->Clear();
					break;

				}
			}
		}
	}

	void SaveWritingState::UpdateText() {
		_current_.setString("Current: " + _tile_->GetUnicode());
		_prompt_.setString("Please Draw: " + target_hiragana);

		_have_trace_ = _tile_->GetUnicode() == target_hiragana;
	}

}