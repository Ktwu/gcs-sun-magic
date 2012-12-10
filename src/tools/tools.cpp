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

		void OutlineSprite(sf::Sprite sprite, sf::Sprite outline, sf::Texture texture_outline, int radius, sf::Color color) {
			sf::IntRect rect = sprite.getTextureRect();
			sf::RenderTexture temp;
			temp.create(rect.width + 2 * radius, rect.height + 2 * radius);
			temp.draw(sprite);

			sf::Image outline_image = temp.getTexture().copyToImage();
			outline_image.flipVertically();
			sf::Vector2u outline_size = outline_image.getSize();

			sf::Image new_image;
			new_image.create(outline_size.x, outline_size.y, sf::Color::Transparent);

			int outline_i, outline_j, i, j, max_i, max_j, min_i, min_j, local_i, local_j;
			for (outline_i = 0; outline_i < (int)outline_size.x; ++outline_i) {
				for (outline_j = 0; outline_j < (int)outline_size.y; ++outline_j) {
					i = outline_i - radius;
					j = outline_j - radius;
					min_i = std::max(0, i - radius);
					max_i = std::min(rect.width, i + radius);
					min_j = std::max(0, j - radius);
					max_j = std::min(rect.height, j + radius);

					float strength = 0;
					for (local_i = min_i; local_i < max_i; ++local_i) {
						for (local_j = min_j; local_j < max_j; ++local_j) {
							if (outline_image.getPixel(local_i, local_j).a > 0)  {
								float distSquared = std::pow((float)(i - local_i), 2.f) + std::pow((float)(j - local_j), 2.f);
								strength += 1.f / std::pow(distSquared, 1.5f);
							}
						}
					}
				
					if (strength > 0) {
						color.a = sf::Uint8(std::min(255.f, 255.f * 1.5f * strength));
						new_image.setPixel(outline_i, outline_j, color);
					}
				}
			}

			texture_outline.loadFromImage(new_image);
			outline.setTexture(texture_outline, true);
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