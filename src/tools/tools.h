#pragma once

#include "stdafx.h"
#include "game.h"

namespace sun_magic {
	namespace tools {

		void ScaleToWindowSize(sf::Sprite& sprite);

		void OutlineSprite(sf::Sprite sprite, sf::Sprite outline, sf::Texture texture_outline, int radius, sf::Color color);

		bool ZSort(GameObject* a, GameObject* b);

		sf::String UTF8ToUTF32(const char* utf8str);

		int LimitBetween(int min, int val, int max);

		sf::Color HSVColor(float h, float s, float v);

		sf::Color Multiply(sf::Color Color, float m);

		sf::FloatRect GetBounds (zinnia::Character* character);

		zinnia::Character* Normalize(zinnia::Character* character);

		zinnia::Character* Resize(zinnia::Character* character, size_t width, size_t height);

		float Center(float range, float local_range);
	}
}