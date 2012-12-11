﻿#include "stdafx.h"
#include "gameasset_manager.h"

#include "assets/gameasset.h"
#include "references/refs.h"
#include "tools/tools.h"

namespace sun_magic {

	GameAssetManager* GameAssetManager::instance_ = NULL;

	sf::String GameAssetManager::hiragana_strings[] = {
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
	std::string GameAssetManager::romaji_strings[] = {
		"a",  "i",   "u",   "e",  "o",
		"ka", "ki",  "ku",  "ke", "ko",
		"sa", "shi", "su",  "se", "so",
		"ta", "chi", "tsu", "te", "to",
		"na", "ni",  "nu",  "ne", "no",
		"ha", "hi",  "fu",  "he", "ho",
		"ma", "mi",  "mu",  "me", "mo",
		"ya",        "yu",  "yo",
		"ra", "ri",  "ru",  "re", "ro",
		"wa",                     "wo",
		"n"
	};
	sf::Color GameAssetManager::symbols_colors[] = {
		tools::HSVColor(0/46.f,1,1),  tools::HSVColor(7/46.f,1,1),	tools::HSVColor(14/46.f,1,1), tools::HSVColor(21/46.f,1,1),	tools::HSVColor(28/46.f,1,1),
		tools::HSVColor(35/46.f,1,1), tools::HSVColor(42/46.f,1,1),	tools::HSVColor(3/46.f,1,1),  tools::HSVColor(10/46.f,1,1),	tools::HSVColor(17/46.f,1,1),
		tools::HSVColor(24/46.f,1,1), tools::HSVColor(31/46.f,1,1),	tools::HSVColor(38/46.f,1,1), tools::HSVColor(45/46.f,1,1),	tools::HSVColor(6/46.f,1,1),
		tools::HSVColor(13/46.f,1,1), tools::HSVColor(20/46.f,1,1),	tools::HSVColor(27/46.f,1,1), tools::HSVColor(34/46.f,1,1),	tools::HSVColor(41/46.f,1,1),
		tools::HSVColor(2/46.f,1,1),  tools::HSVColor(9/46.f,1,1),	tools::HSVColor(16/46.f,1,1), tools::HSVColor(23/46.f,1,1),	tools::HSVColor(30/46.f,1,1),
		tools::HSVColor(37/46.f,1,1), tools::HSVColor(44/46.f,1,1),	tools::HSVColor(5/46.f,1,1),  tools::HSVColor(12/46.f,1,1), tools::HSVColor(19/46.f,1,1),
		tools::HSVColor(26/46.f,1,1), tools::HSVColor(33/46.f,1,1),	tools::HSVColor(40/46.f,1,1), tools::HSVColor(1/46.f,1,1),  tools::HSVColor(8/46.f,1,1),
		tools::HSVColor(15/46.f,1,1),								tools::HSVColor(22/46.f,1,1),								tools::HSVColor(29/46.f,1,1),
		tools::HSVColor(36/46.f,1,1), tools::HSVColor(43/46.f,1,1),	tools::HSVColor(4/46.f,1,1), tools::HSVColor(11/46.f,1,1),	tools::HSVColor(18/46.f,1,1),
		tools::HSVColor(25/46.f,1,1),																							tools::HSVColor(32/46.f,1,1),
		tools::HSVColor(39/46.f,1,1)
	};
	int GameAssetManager::symbols_pos[] = {
			0,  1,  2,  3,  4,
			5,  6,  7,  8,  9,
			10, 11, 12, 13, 14,
			15, 16, 17, 18, 19,
			20, 21, 22, 23, 24,
			25, 26, 27, 28, 29,
			30, 31, 32, 33, 34,
			35,     37,     39,
			40, 41, 42, 43, 44,
			45,             49,
			50
		};

	void GameAssetManager::Init() {
		std::ifstream trace_characters;
		trace_characters.open(refs::tracer::HIRAGANA_TRACE);

		// We need to parse our trace characters out
		zinnia::Character* character;
		std::string parsed_line;
		int line = 1;
		while (!trace_characters.eof()) {
			parsed_line.clear();
			std::getline(trace_characters, parsed_line);
			character = zinnia::createCharacter();
			character->parse(parsed_line.c_str());
			if (parsed_line.size() > 6) {
				sf::String utf32_str = tools::UTF8ToUTF32(character->value());
				if (utf32_str.getSize() > 0)
					trace_characters_[utf32_str[0]] = character;
				else
					std::cout << "Line " << line << " Error: " << parsed_line << std::endl;
			}
			line++;
		}

		// Init maps for hiragana and romaji
		for (int i = 0; i < NUM_SYMBOLS; i++) {
			hiragana_indices_.insert(std::pair<sf::Uint32,int>(hiragana_strings[i][0],i));
		}
		for (int i = 0; i < NUM_SYMBOLS; i++) {
			romaji_indices_.insert(std::pair<std::string,int>(romaji_strings[i],i));
		}
	}

	sf::Texture* GameAssetManager::GetTexture(void* key, std::string file) {
		std::vector<sf::Texture*> vector;
		std::vector<std::string> strings;
		strings.push_back(file);

		GetTextures(key, strings, vector);
		return vector[0];
	}

	void GameAssetManager::GetTextures(void* key, const std::vector<std::string>& texture_keys, std::vector<sf::Texture*>& textures) {
		GetAssets(key, texture_keys, textures, this->textures_, this->texture_holders_, this->LoadTexture);
	}

	void GameAssetManager::ReturnTextures(void* key) {
		ReturnAssets(key, this->textures_, this->texture_holders_);
	}

	sf::Font* GameAssetManager::GetFont(void* key, std::string file) {
		std::vector<sf::Font*> vector;
		std::vector<std::string> strings;
		strings.push_back(file);

		GetFonts(key, strings, vector);
		return vector[0];
	}

	void GameAssetManager::GetFonts(void* key, const std::vector<std::string>& font_keys, std::vector<sf::Font*>& fonts) {
		GetAssets(key, font_keys, fonts, this->fonts_, this->font_holders_, this->LoadFont);
	}

	void GameAssetManager::ReturnFonts(void* key) {
		ReturnAssets(key, this->fonts_, this->font_holders_);
	}

	void GameAssetManager::CleanUnusedTextures() {
		CleanUnusedAssets(textures_);
	}

	void GameAssetManager::CleanUnusedFonts() {
		CleanUnusedAssets(fonts_);
	}

	void GameAssetManager::GetTraceableCharacters(std::vector<sf::Uint32>& characters) {
		for (std::hash_map<sf::Uint32, zinnia::Character*>::iterator iter =
				trace_characters_.begin(); iter != trace_characters_.end(); iter++) {
			characters.push_back(iter->first);
		}
	}

	zinnia::Character* GameAssetManager::GetTraceCharacter(sf::Uint32 utf32_character) {
		return trace_characters_[utf32_character];
	}

		sf::Sprite GameAssetManager::GetHiraganaSprite(sf::String hiragana, sf::Texture* sprites) {
		int index = GetHiraganaIndex(hiragana);
		int pos = symbols_pos[index];

		sf::Vector2u size = sprites->getSize();
		size.x /= ROW_SIZE;
		size.y /= COL_SIZE;

		int x = pos % ROW_SIZE;
		int y = pos / ROW_SIZE;

		return sf::Sprite(*sprites, sf::IntRect(size.x * x, size.y * y, size.x, size.y));
	}

	int GameAssetManager::GetHiraganaIndex(sf::String character) {
		std::hash_map<sf::Uint32,int>::iterator iter = hiragana_indices_.find(character[0]);
		if (iter == hiragana_indices_.end())
			return -1;
		else
			return iter->second;
	}
	int GameAssetManager::GetRomajiIndex(std::string str) {
		std::hash_map<std::string,int>::iterator iter = romaji_indices_.find(str);
		if (iter == romaji_indices_.end())
			return -1;
		else
			return iter->second;
	}

	sf::String GameAssetManager::HiraganaToRomaji(sf::String word) {
		sf::String translation;
		int size = word.getSize();
		for (int i = 0; i < size; i++) {
			int index = GetHiraganaIndex(word[i]);
			if (index >= 0)
				translation += romaji_strings[index];
			else
				translation += "?";
		}
		return translation;
	}
	sf::String GameAssetManager::RomajiToHiragana(sf::String word) {
		sf::String translation;
		std::string str = word.toAnsiString();
		int size = str.size();
		for (int i = 0; i < size; i++) {
			int index = GetRomajiIndex(str.substr(i, 1));
			if (index < 0) {
				index = GetRomajiIndex(str.substr(i, 2));
				if (index < 0)
					index = GetRomajiIndex(str.substr(i, 3));
			}
			if (index >= 0)
				translation += hiragana_strings[index];
			else
				translation += "?";
		}
		return translation;
	}
}