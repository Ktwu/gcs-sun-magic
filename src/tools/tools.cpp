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

		sf::Color Multiply(sf::Color color, float m) {
			return sf::Color(sf::Uint8(m * color.r), sf::Uint8(m * color.g), sf::Uint8(m * color.b), color.a);
		}

		sf::FloatRect GetBounds (zinnia::Character* character) {
			float minX = 10000.f;
			float maxX = -100000.f;
			float minY = minX;
			float maxY = maxX;

			float x, y;

			for (size_t i = 0; i < character->strokes_size(); i++) {
				size_t points = character->stroke_size(i);
				for (size_t j = 0; j < points; j++) {
					x = character->x(i, j);
					y = character->y(i, j);
					minX = std::min(minX, x);
					maxX = std::max(maxX, x);
					minY = std::min(minY, y);
					maxY = std::max(maxY, y);
				}
			}

			std::cout << minX << " " << maxX << " " << minY << " " << maxY << "\n";

			return sf::FloatRect(minX, minY, maxX - minX, maxY - minY);
		}

		zinnia::Character* Normalize(zinnia::Character* character) {
			sf::FloatRect bounds = GetBounds(character);

			// Create a zinnia character with the strokes centered.
			zinnia::Character* new_char =  zinnia::createCharacter();
			new_char->set_height(std::max(bounds.height, bounds.width));
			new_char->set_width(new_char->height());

			float x_offset = bounds.width < bounds.height ? Center(bounds.height, bounds.width) : 0;
			float y_offset = bounds.height < bounds.width ? Center(bounds.width, bounds.height) : 0;

			for (size_t i = 0; i < character->strokes_size(); i++) {
				size_t points = character->stroke_size(i);
				for (size_t j = 0; j < points; j++) {
					new_char->add(i, (int)(character->x(i, j) - bounds.left + x_offset), (int)(character->y(i, j) - bounds.top + y_offset));
				}
			}
		
			char buff[1024];
			character->toString(buff, 1024);
			//std::cout << "old " << buff << "\n";
			new_char->toString(buff, 1024);
			//std::cout << "new " << buff << "\n";

			return new_char;
		}

		zinnia::Character* Resize(zinnia::Character* character, size_t width, size_t height) {
			if (character == NULL ||
				(character->height() == height && character->width() == width))
				return character;

			sf::Vector2f size = sf::Vector2f((float)width, (float)height);
			zinnia::Character* new_char = zinnia::createCharacter();
			new_char->set_width(width);
			new_char->set_height(height);

			float xRatio = size.x / character->width();
			float yRatio = size.y / character->height();

			for (size_t i = 0; i < character->strokes_size(); i++) {
				size_t points = character->stroke_size(i);
				for (size_t j = 0; j < points; j++) {
					new_char->add(i, (int)(character->x(i, j) * xRatio), (int)(character->y(i, j) * yRatio));
				}
			}

			return new_char;
		}

		float Center(float range, float local_range) {
			return (range - local_range) / 2;
		}

	}
}