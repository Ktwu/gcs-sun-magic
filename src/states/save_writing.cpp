#include "stdafx.h"
#include "save_writing.h"

#include "game.h"
#include "assets/gameasset_manager.h"
#include "events/event_manager.h"
#include "references/file_refs.h"
#include "references/hiragana_refs.h"
#include "ui/character_tile.h"

namespace sun_magic {

	SaveWritingState::SaveWritingState() {
		sf::Vector2u size = Game::GetInstance()->GetWindow()->getSize();
		tile_ = new CharacterTile(size.x * 0.5f - 150, size.y * 0.5f - 150, 300, 300);

		trace_output_.open(file_refs::HIRAGANA_OUTPUT);

		target_index = 0;
		character_ = GameAssetManager::GetInstance()->GetTraceCharacter((hiragana::id) target_index);
		tile_->SetTraceCharacter(character_, hiragana::refs[(hiragana::id) target_index]);
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
		trace_output_.close();
		delete tile_;
	}
	
	void SaveWritingState::RegisterState(MachineState<GameState>* previous_state) {
		Game::GetInstance()->GetEventManager()->AddGameObject(tile_);
		tile_->Register();
		Game::GetInstance()->GetEventManager()->RegisterListener(Event::E_KEY_RELEASED, this);
	}

	void SaveWritingState::UnregisterState(MachineState<GameState>* previous_state) {
		Game::GetInstance()->GetEventManager()->RemoveGameObject(tile_);
		tile_->Unregister();
		Game::GetInstance()->GetEventManager()->UnregisterListener(Event::E_KEY_RELEASED, this);
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

	void SaveWritingState::ProcessEvent(Event *event) {
		switch (event->type) {
		case Event::E_KEY_RELEASED:
			{
				KeyEvent *key_event = (KeyEvent*)event;
				switch (key_event->key) {

				case Keyboard::Space:
					tile_->Clear();
					tile_->SetAnimationStroke(0);
					break;

				case Keyboard::S:
					// Save our output
					std::stringstream ss;
					ss << target_index;
					tile_->GetCharacter()->set_value(ss.str().c_str());
					tile_->GetCharacter()->toString(buff, size);
					std::string better_string = std::string(buff);

					size_t replace_pos = better_string.find("stroeks");
					better_string.replace(replace_pos, sizeof("stroeks"), "strokes");

					trace_output_ << better_string << "\n";
					std::cout << better_string << "\n";

					// Switch to the next character to draw
					target_index = (target_index + 1) % hiragana::NUM_HIRAGANA;
					character_ = GameAssetManager::GetInstance()->GetTraceCharacter((hiragana::id) target_index);
					tile_->SetTraceCharacter(character_, hiragana::refs[(hiragana::id) target_index]);

					// Clear things up
					tile_->Clear();
					tile_->SetAnimationStroke(0);
					break;

				}
			}
		}
	}

	void SaveWritingState::UpdateText() {
		current_.setString("Current: " + tile_->GetUnicode());
		prompt_.setString("Please Draw: " + hiragana::refs[(hiragana::id) target_index]);
	}

}