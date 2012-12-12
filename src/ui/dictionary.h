#pragma once

#include "stdafx.h"
#include "animations/move_animation.h"
#include "events/event.h"
#include "objects/game_object.h"
#include "tools/sfm.h"
#include "ui/character_tile.h"

namespace sun_magic {

	class Dictionary : public GameObject, public EventListener {
	public:
		struct DictionaryEntry {
			sf::Color outline;
			sf::Sprite sprite;
			sf::String romanji;
			CharacterTile *tile;
		};

		Dictionary(float hide_x, float hide_y, float show_x, float show_y, float width, float height, bool is_vertical);
		~Dictionary();

		void AddWord(sf::String word);
		void Clear();

		void Register();
		void Unregister();

		void Update(float elapsed_time);
		void Draw(sf::RenderTarget *target);

		void ProcessEvent(Event event);

	protected:
		std::vector<DictionaryEntry> entries_;
		sf::Vector2f hide_pos_;
		sf::Vector2f show_pos_;

		bool is_vertical_;

		sf::Font* font;
		sf::Font* english_font;

		MoveAnimation move_animation_;
	};

}