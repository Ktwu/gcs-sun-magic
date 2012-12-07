#pragma once

#include "stdafx.h"
#include "tools/tools.h"

namespace sun_magic {

	template<class T>
	class GameAsset;

	class GameAssetManager {
	public:
		static const int NUM_SYMBOLS = 46;
		static const int ROW_SIZE = 5;
		static const int COL_SIZE = 11;
		static sf::String hiragana_strings[];
		static std::string romaji_strings[];
		static sf::Color symbols_colors[];
		static int symbols_pos[];

		GameAssetManager() {}
		~GameAssetManager() {}

		static GameAssetManager* GetInstance() {
			if (GameAssetManager::instance_ == NULL) {
				GameAssetManager::instance_ = new GameAssetManager();
			}
			return GameAssetManager::instance_;
		}

		void Init();

		sf::Texture* GetTexture(std::string textureName);
		void ReturnTexture(std::string textureName);
		void CleanUnusedTextures();

		sf::Sprite GetHiraganaSprite(sf::String hiragana, sf::Texture* sprites);

		sf::Font* GetFont(std::string fontName);
		void ReturnFont(std::string fontName);
		void CleanUnusedFonts();

		void GetTraceableCharacters(std::vector<sf::Uint32>& characters);
		zinnia::Character* GetTraceCharacter(sf::Uint32 utf32_character);

		int GetHiraganaIndex(sf::String character);
		int GetRomajiIndex(std::string str);
		sf::String HiraganaToRomaji(sf::String hiragana);
		sf::String RomajiToHiragana(sf::String romaji);

	private:
		static GameAssetManager* instance_;

		sf::Mutex lock_;

		std::hash_map<std::string, GameAsset<sf::Texture>*> textures_;
		std::hash_map<std::string, GameAsset<sf::Font>*> fonts_;

		std::hash_map<sf::Uint32, zinnia::Character*> trace_characters_;

		std::hash_map<sf::Uint32,int> hiragana_indices_;
		std::hash_map<std::string,int> romaji_indices_;
	};
}