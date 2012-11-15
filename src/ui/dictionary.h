#pragma once

#include "stdafx.h"
#include "objects/game_object.h"

namespace sun_magic {

	class Dictionary : public GameObject {
	public:
		Dictionary(float x, float y, float width, float height);
		~Dictionary();

		void Register();
		void Unregister();

		void Update(float elapsed_time);
		void Draw(sf::RenderTarget *target);
		void DictWords(sf::RenderTarget *target, std::vector<std::string> words);
	};

}
