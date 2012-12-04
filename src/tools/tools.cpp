#pragma once

#include "stdafx.h"
#include "tools.h"

namespace sun_magic {
	namespace tools {

		void ScaleToWindowSize(sf::Sprite& sprite) {
			sf::RenderWindow* window = Game::GetInstance()->GetWindow();
			sf::Vector2u window_size = window->getSize();
			sf::Vector2u sprite_size = sprite.getTexture()->getSize();

			sprite.setScale((float)window_size.x / sprite_size.x,
							(float)window_size.y / sprite_size.y);
			sprite.setPosition(0, 0);
		}

		bool ZSort(GameObject* a, GameObject* b) {
			return a->GetZ() > b->GetZ();
		}

		sf::String UTF8ToUTF32(const char* utf8str) {
			sf::Uint32 utf32str[] = {0,0};
			sf::Utf8::toUtf32((unsigned char*)utf8str, (unsigned char*)utf8str + strlen(utf8str), utf32str);
			return sf::String(utf32str);
		}

		int LimitBetween(int min, int val, int max) {
			if (val < min)
				return min;
			if (val > max)
				return max;
			return val;
		}

		/* Creates a HSV space color, H,S,V should be in the range [0,1] */
		sf::Color HSVColor(float h, float s, float v) {
			h *= 6;
			sf::Uint8 c = sf::Uint8(255 * s * v);
			sf::Uint8 x = sf::Uint8(c * (1 - abs(fmod(h, 2.f) - 1)));
			sf::Color color;
			switch ((int)h) {
			case 0:
				return sf::Color(c,x,0);
			case 1:
				return sf::Color(x,c,0);
			case 2:
				return sf::Color(0,c,x);
			case 3:
				return sf::Color(0,x,c);
			case 4:
				return sf::Color(x,0,c);
			case 5:
				return sf::Color(c,0,x);
			default:
				throw "Invalid h value";
			}
		}
	}
}