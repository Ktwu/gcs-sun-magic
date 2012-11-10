﻿#pragma once

#include "stdafx.h"
#include "save_writing.h"

#include "character_tile.h"
#include "event.h"
#include "event_manager.h"
#include "game.h"
#include "game_state.h"
#include "gameasset_manager.h"
#include "file_refs.h"
#include "hiragana_refs.h"
#include "save_writing.h";
#include "machine_state.h"

namespace sun_magic {

	SaveWritingState::SaveWritingState() {
		sf::Vector2u size = Game::GetInstance()->GetWindow()->getSize();
		_tile_ = new CharacterTile(size.x * 0.5f - 150, size.y * 0.5f - 150, 300, 300);

		_trace_output_.open(file_refs::save_hiragana.toAnsiString());

		target_index = 0;
		_character_ = GameAssetManager::GetInstance()->GetTraceCharacter((hiragana::id) target_index);
		_tile_->SetTraceCharacter(_character_, hiragana::refs[(hiragana::id) target_index]);
		_tile_->SetAnimationSpeed(200.0f);
		_tile_->SetAnimationStroke(0);

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
	
	void SaveWritingState::RegisterState(MachineState<ref::GameState>* previous_state) {
		Game::GetInstance()->GetEventManager()->AddGameObject(_tile_);
		_tile_->Register();
		Game::GetInstance()->GetEventManager()->RegisterListener(Event::E_KEY_RELEASED, this);
	}

	void SaveWritingState::UnregisterState(MachineState<ref::GameState>* previous_state) {
		Game::GetInstance()->GetEventManager()->RemoveGameObject(_tile_);
		_tile_->Unregister();
		Game::GetInstance()->GetEventManager()->UnregisterListener(Event::E_KEY_RELEASED, this);
	}

	ref::GameState SaveWritingState::Update(float elapsed_time) {
		_tile_->Update(elapsed_time);
		UpdateText();

		return ref::RECORDING;
	}
	
	void SaveWritingState::PreDraw(sf::RenderTarget *target) {
		target->clear(sf::Color::Black);
	}

	void SaveWritingState::PostDraw(sf::RenderTarget *target) {
		/* Draw tile */
		_tile_->Draw(target);

		/* Draw UI strings */
		target->draw(_prompt_);
		target->draw(_current_);
		target->draw(_save_);
	}

	void SaveWritingState::ProcessEvent(Event *event) {
		switch (event->type) {
		case Event::E_KEY_RELEASED:
			{
				KeyEvent *key_event = (KeyEvent*)event;
				switch (key_event->key) {

				case Keyboard::Space:
					_tile_->Clear();
					_tile_->SetAnimationStroke(0);
					break;

				case Keyboard::S:
					// Save our output
					std::stringstream ss;
					ss << target_index;
					_tile_->GetCharacter()->set_value(ss.str().c_str());
					_tile_->GetCharacter()->toString(buff, size);
					std::string better_string = std::string(buff);

					size_t replace_pos = better_string.find("stroeks");
					better_string.replace(replace_pos, sizeof("stroeks"), "strokes");

					_trace_output_ << better_string << "\n";
					std::cout << better_string << "\n";

					// Switch to the next character to draw
					target_index = (target_index + 1) % hiragana::NUM_HIRAGANA;
					_character_ = GameAssetManager::GetInstance()->GetTraceCharacter((hiragana::id) target_index);
					_tile_->SetTraceCharacter(_character_, hiragana::refs[(hiragana::id) target_index]);

					// Clear things up
					_tile_->Clear();
					_tile_->SetAnimationStroke(0);
					break;

				}
			}
		}
	}

	void SaveWritingState::UpdateText() {
		_current_.setString("Current: " + _tile_->GetUnicode());
		_prompt_.setString("Please Draw: " + hiragana::refs[(hiragana::id) target_index]);
	}

}