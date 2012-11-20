#pragma once

#include "stdafx.h"
#include "events/event.h"
#include "objects/game_object.h"
#include "ui/character_tile.h"

namespace sun_magic {

	/* Widget to keep track of hiragana drawn in a list of words.  When a word is either being
	   written or finished, this fires an event containing the word.  Any incomplete or unknown
	   characters are replaced with "?" */
	class CharacterTileList : public GameObject, public EventListener {
	public:

		CharacterTileList(float x = 0, float y = 0, float width = 300, float height = 300, int num_tiles = 5);
		~CharacterTileList();

		void Clear();
		sf::String GetWord();
	
		void Register();
		void Unregister();
		void Update(float elapsedSeconds);
		void ProcessEvent(Event event);
		void Draw(sf::RenderTarget* target);		

	private:
		std::vector<CharacterTile*> tiles_;
		sf::String word_;
	};

}