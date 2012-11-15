#pragma once

#include "stdafx.h"
#include "events/event.h"
#include "objects/game_object.h"

namespace sun_magic {

	class KeyObject : public GameObject, public EventListener
	{
	public:
		enum KeyState {
			DEFAULT,
			OUTLINED,
			ACTIVE
		};

		KeyObject(float x, float y, sf::Texture& texture, sf::Color outline, sf::String word, bool active = true);
		~KeyObject();

		void SetTexture(const sf::Texture& texture);
		const sf::Texture& GetTexture();

		void SetOutlineColor(sf::Color);
		sf::Color GetOutlineColor();

		void SetWord(sf::String word);
		sf::String GetWord();

		void SetActive(bool active);
		bool IsActive();

		void Register();
		void Unregister();

		void Update(float elapsed_time);
		void Draw(sf::RenderTarget* target);

		void ProcessEvent(Event event);

	protected:
		static const int OUTLINE_WIDTH = 5;
		static const sf::Color OUTLINE_COLORS[16];
		static const KeyObject* REGISTERED_KEY_OBJECTS[16];

		sf::Sprite sprite_;
		sf::Color outline_;
		sf::String word_;
		bool active_;
		bool registered_;
		KeyState state_;
	};
	
	const sf::Color KeyObject::OUTLINE_COLORS[16] = {
		sf::Color::Black
	};
}