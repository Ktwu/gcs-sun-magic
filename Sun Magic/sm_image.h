#pragma once

#include "stdafx.h"

namespace SunMagic {
	namespace Util {
		bool loadSceneBackground(sf::Sprite& sprite, sf::Texture& texture) {
			if (sprite.getTexture() == NULL) {
				texture.loadFromFile(Tutorial::Office::desk);

				sf::Vector2u size = texture.getSize();
				sf::Vector2u windowSize = Game::mainWindow.getSize();

				sprite.setTexture(texture);
				sprite.setScale((float) windowSize.x/size.x, (float) windowSize.y/size.y);
			}
		}
	}
}