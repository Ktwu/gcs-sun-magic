#pragma once

#include "stdafx.h"
#include "events/event.h"
#include "events/gameevent.h"
#include "objects/game_object.h"

namespace sun_magic {

	class Animon : public GameObject, public EventListener
	{
	public:
		enum KeyState {
			DEFAULT,
			OUTLINED,
			ACTIVE
		};

		enum AnimonState {
			HAPPY,
			ANGRY,
			MEH,
			UNINITIALIZED
		};

		Animon(float x, float y, sf::Color outline, sf::String word, bool active = true, bool visible = true);
		~Animon();

		void SetSprite(sf::Sprite Sprite);
		sf::Sprite GetSprite();

		void SetOutlineColor(sf::Color);
		sf::Color GetOutlineColor();

		void SetWord(sf::String word);
		sf::String GetWord();

		sf::Sound& GetCry();

		void SetFocused(bool focused);
		bool IsFocused();

		void SetActive(bool active);
		bool IsActive();

		void SetVisible(bool visible);
		bool IsVisible();

		void LoadState(AnimonState state);

		void Register();
		void Unregister();

		void Update(float elapsed_time);
		void Draw(sf::RenderTarget* target);

		void ProcessEvent(Event event);

	protected:
		static const int OUTLINE_WIDTH = 5;
		static void ThreadLoad(Animon* animon);

		AnimonState animon_state_;

		sf::Sprite sprite_;

		sf::Sprite sprite_outline_;
		sf::Color outline_;
		sf::Texture texture_outline_;

		sf::Sound cry_;

		sf::String word_;
		bool active_;
		bool focused_;
		bool registered_;
		bool visible_;
		KeyState state_;

		sf::Texture* meh_texture_;
		sf::Texture* angry_texture_;
		sf::Texture* happy_texture_;
	};
	
}