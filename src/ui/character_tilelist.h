#pragma once

#include "stdafx.h"
#include "events/event.h"
#include "objects/game_object.h"
#include "ui/character_tile.h"

namespace sun_magic {

	class CharacterTileList : public GameObject, public EventListener {
	public:

		CharacterTileList(float x = 0, float y = 0, float width = 300, float height = 300, int num_tiles = 5);
		~CharacterTileList();

		void Clear();
	
		void Register();
		void Unregister();
		void Update(float elapsedSeconds);
		void ProcessEvent(Event event);
		void Draw(sf::RenderTarget* target);		

	private:
		std::vector<CharacterTile*> tiles_;
	};

}