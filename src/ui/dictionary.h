#pragma once

#include "stdafx.h"
#include "events/event.h"
#include "objects/game_object.h"

namespace sun_magic {

	class Dictionary : public GameObject, public EventListener {
	public:
		struct DictionaryEntry {
			sf::Color outline;
			sf::Texture *texture;	// Picture of object
		};

		Dictionary(int hide_x, int hide_y, int show_x, int show_y, int width, int height);
		~Dictionary();

		void AddWord(sf::String word, sf::Texture *texture);
		sf::Color GetWordColor(sf::String word);

		void Register();
		void Unregister();

		void Update(float elapsed_time);
		void Draw(sf::RenderTarget *target);

		void ProcessEvent(Event event);

	protected:
		static sf::Color GetColor();

		std::map<sf::String, DictionaryEntry> entries_;
		sf::Vector2f hide_pos_;
		sf::Vector2f show_pos_;
		sf::Vector2f target_pos_;
		float animate_speed_;
	};

}