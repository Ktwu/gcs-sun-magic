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

		sf::String UTF8ToUTF32(const char* utf8str) {
			sf::Uint32 utf32str[] = {0,0};
			sf::Utf8::toUtf32((unsigned char*)utf8str, (unsigned char*)utf8str + strlen(utf8str), utf32str);
			return sf::String(utf32str);
		}
	}
}