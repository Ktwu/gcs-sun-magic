#include "stdafx.h"
#include "events\event_manager.h"
#include "ui/character_tile.h"
#include "ui/character_tilelist.h"
#include "game.h"

namespace sun_magic {

	CharacterTileList::CharacterTileList(float x, float y, float width, float height, int num_tiles) :
		GameObject(x, y, width, height)
	{
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
	}

	void CharacterTileList::Draw(sf::RenderTarget* target) {
		for (int i = 0; i < tiles_.size(); ++i) {
			tiles_[i]->Draw(target);
		}
	}
}