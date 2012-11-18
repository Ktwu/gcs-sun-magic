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

	void CharacterTileList::Register() {
		EventManager* manager = Game::GetInstance()->GetEventManager();
		for (int i = 0; i < tiles_.size(); ++i) {
			manager->AddGameObject(tiles_[i]);
			manager->RegisterListener(Event::E_MOUSE_RELEASED, this, tiles_[i]);
		}
	}

	void CharacterTileList::Unregister() {
		EventManager* manager = Game::GetInstance()->GetEventManager();
		for (int i = 0; i < tiles_.size(); ++i) {
			manager->AddGameObject(tiles_[i]);
			manager->UnregisterListener(Event::E_MOUSE_RELEASED, this, tiles_[i]);
		}
	}

	void CharacterTileList::Update(float elapsed_time) {
	}

	void CharacterTileList::ProcessEvent(Event event) {
		// Go through all of our tiles and process their characters
		Event hiragana_event;
		sf::String unicode;
		for (int i = 0; i < tiles_.size(); ++i) {
			unicode = tiles_[i]->GetUnicode();
			if (unicode.isEmpty())
				break;

			hiragana_event.message += unicode;
		}

		// Only launch an event if we have a word to even report
		if (hiragana_event.message.isEmpty()) {
			return;
		}

		hiragana_event.focus = this;
		hiragana_event.type = Event::E_HIRAGANA_DRAWN;

		EventManager* manager = Game::GetInstance()->GetEventManager();
		manager->AddEvent(hiragana_event);
	}

	void CharacterTileList::Draw(sf::RenderTarget* target) {
		for (int i = 0; i < tiles_.size(); ++i) {
			tiles_[i]->Draw(target);
		}
	}
}