#pragma once

#include "stdafx.h"
#include "events/event.h"
#include "objects/game_object.h"
#include "ui/style.h"

namespace sun_magic {

	class Button : public GameObject, public EventListener {
	public:
		enum ButtonState {
			NEUTRAL,
			HOVERED,
			PRESSED,
		};

		Button(float x = 0, float y = 0, float width = 300, float height = 100, sf::String text = "");
		~Button();

		Style* GetStyle();

		sf::String GetString();
		void SetString(sf::String string);

		void Register();
		void Unregister();

		void Update(float elapsed_time);
		void Draw(sf::RenderTarget* target);

		void ProcessEvent(Event event);

	protected:
		sf::String string_;
		Style style_;

		int state_;
	};

}