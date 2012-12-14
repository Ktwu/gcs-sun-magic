#include "stdafx.h"
#include "ui/character_tilelist.h"

#include "assets/gameasset_manager.h"
#include "events/event_manager.h"
#include "tools/tools.h"
#include "ui/character_tile.h"
#include "game.h"

namespace sun_magic {

	CharacterTileList::CharacterTileList(float x, float y, float width, float height, float start_x, float start_y, int num_tiles) :
		UiGroup(x, y, width, height)
	{
		z_ = -1;
		float tile_width = (width-start_x) / num_tiles;
		
		for (int i = 0; i < num_tiles; ++i) {
			CharacterTile *tile = new CharacterTile(start_x + i*tile_width, start_y, tile_width, height-start_y);
			tile->GetTileStyle()->SetGuideColor(sf::Color::Transparent);
			tiles_.push_back(tile);
			tiles_[i]->SetZ(0);
			UiAdd(tiles_[i]);
		}
	}

	CharacterTileList::~CharacterTileList() {
		for (size_t i = 0; i < tiles_.size(); ++i)
			delete tiles_[i];

		tiles_.clear();
		UiClear();
	}
	
	void CharacterTileList::SetZ(float z) {
		GameObject::SetZ(z);
		for (size_t i = 0; i < tiles_.size(); ++i) {
			tiles_[i]->SetZ(z - 0.001f);
		}
	}

	void CharacterTileList::Clear() {
		for (size_t i = 0; i < tiles_.size(); ++i) {
			tiles_[i]->Clear();
			tiles_[i]->SetAnimationStroke(-1);
			tiles_[i]->SetTraceCharacter(NULL);
		}
	}

	sf::String CharacterTileList::GetWord() {
		return word_;
	}
	void CharacterTileList::SetTraceWord(sf::String word) {
		GameAssetManager* manager = GameAssetManager::GetInstance();
		for (size_t i = 0; i < tiles_.size(); ++i) {
			zinnia::Character* character = manager->GetTraceCharacter(word[i]);
			character = tools::Resize(character, (size_t)tiles_[i]->GetSize().x, (size_t)tiles_[i]->GetSize().y);
			tiles_[i]->SetTraceCharacter(character);
			tiles_[i]->SetAnimationStroke(0);
		}
	}

	void CharacterTileList::Register() {
		UiGroup::Register();
		EventManager* manager = Game::GetInstance()->GetEventManager();
		for (size_t i = 0; i < tiles_.size(); ++i) {
			manager->RegisterListener(Event::E_HIRAGANA_DRAWN, this, tiles_[i]);
		}
	}

	void CharacterTileList::Unregister() {
		UiGroup::Unregister();
		EventManager* manager = Game::GetInstance()->GetEventManager();
		for (size_t i = 0; i < tiles_.size(); ++i) {
			manager->UnregisterListener(Event::E_HIRAGANA_DRAWN, this, tiles_[i]);
		}
	}

	void CharacterTileList::ProcessEvent(Event event) {
		/*
		// Go through all of our tiles and process their characters into a message
		sf::String unicode;
		event.message.clear();
		word_.clear();

		for (size_t i = 0; i < tiles_.size(); ++i) {
			// Do we have anything drawn into the tile?
			// If not then we're not actively working in it.
			if (tiles_[i]->GetCharacter()->strokes_size() == 0)
				break;

			unicode = tiles_[i]->GetUnicode();
			if (unicode.isEmpty())
				unicode = "?";

			event.message += unicode;
		}

		// Only launch an event if we have a word to even report
		if (event.message.isEmpty())
			return;

		word_ = event.message;
		event.focus = this;
		event.type = Event::E_HIRAGANA_DRAWN;

		EventManager* manager = Game::GetInstance()->GetEventManager();
		manager->AddEvent(event);
		*/
	}

}