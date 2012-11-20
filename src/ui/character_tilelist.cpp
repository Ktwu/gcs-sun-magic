#include "stdafx.h"
#include "events\event_manager.h"
#include "ui/character_tile.h"
#include "ui/character_tilelist.h"
#include "game.h"

namespace sun_magic {

	CharacterTileList::CharacterTileList(float x, float y, float width, float height, int num_tiles) :
		GameObject(x, y, width, height)
	{
		z_ = -1;
		float tile_width = width / num_tiles;
		
		for (int i = 0; i < num_tiles; ++i) {
			tiles_.push_back(new CharacterTile(x + i*tile_width, y, tile_width, height));
		}
	}

	CharacterTileList::~CharacterTileList() {
		tiles_.clear();
	}

	void CharacterTileList::Clear() {
		for (int i = 0; i < tiles_.size(); ++i) {
			tiles_[i]->Clear();
		}
	}

	sf::String CharacterTileList::GetWord() {
		return word_;
	}

	void CharacterTileList::Register() {
		EventManager* manager = Game::GetInstance()->GetEventManager();
		for (int i = 0; i < tiles_.size(); ++i) {
			manager->AddGameObject(tiles_[i]);
			manager->RegisterListener(Event::E_HIRAGANA_DRAWN, this, tiles_[i]);
		}
	}

	void CharacterTileList::Unregister() {
		EventManager* manager = Game::GetInstance()->GetEventManager();
		for (int i = 0; i < tiles_.size(); ++i) {
			manager->AddGameObject(tiles_[i]);
			manager->UnregisterListener(Event::E_HIRAGANA_DRAWN, this, tiles_[i]);
		}
	}

	void CharacterTileList::Update(float elapsed_time) {
	}

	void CharacterTileList::ProcessEvent(Event event) {
		// Go through all of our tiles and process their characters into a message
		sf::String unicode;
		event.message.clear();
		word_.clear();

		for (int i = 0; i < tiles_.size(); ++i) {
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
	}

	void CharacterTileList::Draw(sf::RenderTarget* target) {
		for (int i = 0; i < tiles_.size(); ++i) {
			tiles_[i]->Draw(target);
		}
	}
}