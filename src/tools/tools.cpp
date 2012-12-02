#pragma once

#include "stdafx.h"
#include "tools.h"

namespace sun_magic {
	namespace tools {
		
		const int NUM_HIRAGANA = 46;
		sf::String symbols_hiragana[] = {
			L"あ", L"い", L"う", L"え", L"お",
			L"か", L"き", L"く", L"け", L"こ",
			L"さ", L"し", L"す", L"せ", L"そ",
			L"た", L"ち", L"つ", L"て", L"と",
			L"な", L"に", L"ぬ", L"ね", L"の",
			L"は", L"ひ", L"ふ", L"へ", L"ほ",
			L"ま", L"み", L"む", L"め", L"も",
			L"や", L"ゆ", L"よ",
			L"ら", L"り", L"る", L"れ", L"ろ",
			L"わ", L"を",
			L"ん"
		};
		sf::String symbols_romaji[] = {
			"a", "i", "u", "e", "o",
			"ka", "ki", "ku", "ke", "ko",
			"sa", "shi", "su", "se", "so",
			"ta", "chi", "tsu", "te", "to",
			"na", "ni", "nu", "ne", "no",
			"ha", "hi", "fu", "he", "ho",
			"ma", "mi", "mu", "me", "mo",
			"ya", "yu", "yo",
			"ra", "ri", "ru", "re", "ro",
			"wa", "wo",
			"n"
		};

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

		bool ContainsSubstringAt(sf::String main, sf::String sub, int pos) {
			int i;
			for (i = 0; (pos+i) < main.getSize() && i < sub.getSize(); ++i) {
				if (main[pos+i] != sub[i])
					return false;
			}
			return (i == sub.getSize());
		}

		sf::String Translate(sf::String word, sf::String from[], sf::String to[]) {
			int size = word.getSize();
			sf::String translation;

			int i, j;
			for (i = 0; i < size;) {
				for (j = 0; j < NUM_HIRAGANA; ++j) {
					if (ContainsSubstringAt(word, from[j], i)) {
						translation += to[j];
						i += from[j].getSize();
						break;
					}
				}

				if (j == NUM_HIRAGANA) {
					translation += "?";
					i += 1;
				}
			}

			return translation;
		}

		sf::String HiraganaToRomaji(sf::String hiragana_word) {
			return Translate(hiragana_word, symbols_hiragana, symbols_romaji);
		}

		sf::String RomajiToHiragana(sf::String romaji_word) {
			return Translate(romaji_word, symbols_romaji, symbols_hiragana);
		}

		int LimitBetween(int min, int val, int max) {
			if (val < min)
				return min;
			if (val > max)
				return max;
			return val;
		}
	}
}