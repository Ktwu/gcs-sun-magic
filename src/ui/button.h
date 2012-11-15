#pragma once

#include "stdafx.h"
#include "events/event.h"
#include "objects/game_object.h"

namespace sun_magic {

	class Button : public GameObject, public EventListener {
	public:
		enum ButtonState {
			NEUTRAL,
			HOVERED,
			PRESSED,
		};

		Button(float x = 0, float y = 0, float width = 300, float height = 100, sf::String text = "", sf::Texture *texture = NULL);
		~Button();

		sf::String GetString();
		void SetString(sf::String string);

		sf::Texture* GetTexture();
		void SetTexture(sf::Texture *texture);

		void Register();
		void Unregister();

		void Update(float elapsed_time);
		void Draw(sf::RenderTarget* target);

		void ProcessEvent(Event *event);

	private:
		sf::String string_;
		sf::Texture *texture_;
		sf::Color background_;
		sf::Color border_;
		sf::Color hoverbackground_;
		sf::Color hover_border_;
		sf::Color pressedbackground_;
		sf::Color pressed_border_;

		int state_;
	};

}