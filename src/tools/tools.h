#pragma once

#include "stdafx.h"
#include "game.h"

namespace sun_magic {
	namespace tools {

		void ScaleToWindowSize(sf::Sprite& sprite);

		bool ZSort(GameObject* a, GameObject* b);

		sf::String UTF8ToUTF32(const char* utf8str);

		bool ContainsSubstringAt(sf::String main, sf::String sub, int pos);
		sf::String HiraganaToRomaji(sf::String hiragana);
		sf::String RomajiToHiragana(sf::String romaji);

		int LimitBetween(int min, int val, int max);

		sf::Sprite GetHiraganaSprite(sf::String hiragana, sf::Texture* sprites);
	}
}