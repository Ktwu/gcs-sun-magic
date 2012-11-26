#pragma once

#include "stdafx.h"
#include "objects/game_object.h"

namespace sun_magic {

	class Label : public GameObject {
	public:

		Label(float x = 0, float y = 0, float width = 300, float height = 100, sf::String text = "", sf::Texture *texture = NULL);
		~Label();

		sf::String GetString();
		void SetString(sf::String string);

		sf::Texture* GetTexture();
		void SetTexture(sf::Texture *texture);

		void Register();
		void Unregister();

		void Update(float elapsed_time);
		void Draw(sf::RenderTarget* target);

	private:
		sf::String string_;
		sf::Texture *texture_;
		sf::Color background_;
		sf::Color border_;
	};

}