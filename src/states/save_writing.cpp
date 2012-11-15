#include "stdafx.h"
#include "save_writing.h"

#include <ctime>

#include "game.h"
#include "assets/gameasset_manager.h"
#include "events/event_manager.h"
#include "references/file_refs.h"
#include "ui/character_tile.h"

namespace sun_magic {

	SaveWritingState::SaveWritingState() {
		sf::Vector2u size = Game::GetInstance()->GetWindow()->getSize();
		tile_ = new CharacterTile(size.x * 0.5f - 150, size.y * 0.5f - 150, 300, 300);

		target_index = 0;
		GameAssetManager::GetInstance()->GetTraceableCharacters(traceable_characters_);
		tile_->SetTraceCharacter(GameAssetManager::GetInstance()->GetTraceCharacter(traceable_characters_[target_index]));
		tile_->SetAnimationSpeed(200.0f);
		tile_->SetAnimationStroke(0);

		if (!font_.loadFromFile(file_refs::MSMINCHO)) {
			std::cerr << "ERROR: Unable to load font " << file_refs::MSMINCHO << std::endl;
		}
		unsigned int width = size.x;
		unsigned int y = size.y - 200;

		/* Set up the strings to display in the scene */
		prompt_.setFont(font_);
		prompt_.setColor(sf::Color(200, 200, 200));
		prompt_.setPosition(width * 0.30f, (float)y);
		current_.setFont(font_);
		current_.setColor(sf::Color(200, 200, 200));
		current_.setPosition(width * 0.30f, (float)y + 50);
		save_.setFont(font_);
		save_.setColor(sf::Color(200, 200, 200));
		save_.setPosition(width * 0.30f, (float)y + 100);
		save_.setString("Press 'S' to save");
	}

	SaveWritingState::~SaveWritingState() {
		delete tile_;
	}
	
	void SaveWritingState::RegisterState(MachineState<GameState>* previous_state) {
		Game::GetInstance()->GetEventManager()->AddGameObject(tile_);
		Game::GetInstance()->GetEventManager()->RegisterListener(Event::E_KEY_RELEASED, this);
		
		// Create filename based on date
		time_t t = time(0);
		struct tm *now = localtime(&t);
		std::stringstream ss;
		ss << file_refs::HIRAGANA_OUTPUT_BASE << (now->tm_year + 1900) <<
			'-' << (now->tm_mon + 1) << '-' <<  now->tm_mday << "_" <<
			now->tm_hour << "-" << now->tm_min << "-" << now->tm_sec << ".txt";
		std::cout << "Recording to " << ss.str() << std::endl;
		trace_output_ = fopen(ss.str().c_str(), "w+,ccs=UTF-8");
	}

	void SaveWritingState::UnregisterState(MachineState<GameState>* next_state) {
		Game::GetInstance()->GetEventManager()->RemoveGameObject(tile_);
		Game::GetInstance()->GetEventManager()->UnregisterListener(Event::E_KEY_RELEASED, this);

		fclose(trace_output_);
	}

	GameState SaveWritingState::Update(float elapsed_time) {
		UpdateText();

		return RECORDING;
	}
	
	void SaveWritingState::PreDraw(sf::RenderTarget *target) {
		target->clear(sf::Color::Black);
	}

	void SaveWritingState::PostDraw(sf::RenderTarget *target) {
		/* Draw tile */
		tile_->Draw(target);

		/* Draw UI strings */
		target->draw(prompt_);
		target->draw(current_);
		target->draw(save_);
	}

	void SaveWritingState::ProcessEvent(Event event) {
		switch (event.type) {
		case Event::E_KEY_RELEASED:
			switch (event.key.code) {
			case Keyboard::Space:
				tile_->Clear();
				tile_->SetAnimationStroke(0);
				break;

			case Keyboard::S:
				// Save our output
				std::stringstream ss;
				ss << target_index;
				tile_->GetCharacter()->toString(buff, size);
				std::string better_string = std::string(buff);

				// Replace stroeks typo in the output
				size_t replace_pos = better_string.find("stroeks");
				better_string.replace(replace_pos, sizeof("stroeks"), "strokes");

				// Convert the string to wide char and also write the value
				// for some reason value is not included in toString
				size_t index = better_string.find("value") + 6;
				std::wstring pre = sf::String(better_string.substr(0, index)).toWideString();
				std::wstring value = tile_->GetUnicode().toWideString();
				std::wstring post = sf::String(better_string.substr(index)).toWideString();
				fwrite(pre.c_str(), pre.size() * sizeof(wchar_t), 1, trace_output_);
				fwrite(value.c_str(), value.size() * sizeof(wchar_t), 1, trace_output_);
				fwrite(post.c_str(), post.size() * sizeof(wchar_t), 1, trace_output_);
				fwprintf(trace_output_, L"\n");
				fflush(trace_output_);
				std::cout << better_string << "\n";

				// Switch to the next character to draw
				target_index = (target_index + 1) % traceable_characters_.size();
				character_ = GameAssetManager::GetInstance()->GetTraceCharacter(traceable_characters_[target_index]);
				tile_->SetTraceCharacter(character_);

				// Clear things up
				tile_->Clear();
				tile_->SetAnimationStroke(0);
				break;

			}
			break;
		}
	}

	void SaveWritingState::UpdateText() {
		current_.setString("Current: " + tile_->GetUnicode());
		prompt_.setString("Please Draw: " + tile_->GetTraceUnicode());
	}

}