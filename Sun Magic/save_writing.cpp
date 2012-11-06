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

		//zinnia::Character *character = zinnia::Character::create();
		//character->parse("(character (value い) (width 300) (height 300) (strokes ((56 63)(43 213)(67 259)(94 243)) ((213 66)(231 171)(208 217))))");
		//_tile_->SetTraceCharacter(NULL, sf::String(L"い"));

		target_index = 0;
		target_hiragana = CharacterTile::GetRecognizer()->value(target_index);
	
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
	}

	SaveWritingState::~SaveWritingState() {
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
					target_index = (target_index + 1) % CharacterTile::GetRecognizer()->size();
					std::cout << CharacterTile::GetRecognizer()->size() << "\n";
					target_hiragana = _tile_->UTF8ToUTF32(CharacterTile::GetRecognizer()->value(target_index));
					_tile_->Clear();
					break;
				}
			}
		}
	}

	void SaveWritingState::UpdateText() {
		_current_.setString("Current: " + _tile_->GetUnicode());
		_prompt_.setString("Please Draw: " + target_hiragana);

		//char buff[256];
		//_tile_->GetCharacter()->toString(buff, 256);
		//std::cout << buff << "\n";
	}

}