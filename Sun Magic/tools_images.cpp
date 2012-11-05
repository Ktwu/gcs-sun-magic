#pragma once

#include "stdafx.h"
#include "game.h"

namespace sun_magic {
	namespace tools {
		namespace images {

			void ScaleToWindowSize(sf::Sprite& sprite) {
				sf::RenderWindow& window = Game::GetInstance()->main_window_;
				sf::Vector2u sprite_size = sprite.getTexture()->getSize();
				sf::Vector2u window_size = window.getSize();

				sprite.setScale((float)window_size.x / sprite_size.x,
							    (float)window_size.y / sprite_size.y);
				sprite.setPosition(0, 0);
			}

		}
	}
}