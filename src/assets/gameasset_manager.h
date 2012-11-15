#pragma once

#include "stdafx.h"

namespace sun_magic {

	template<class T>
	class GameAsset;

	class GameAssetManager {
	public:
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

		void GetTraceableCharacters(std::vector<sf::Uint32>& characters);
		zinnia::Character* GetTraceCharacter(sf::Uint32 utf32_character);

	private:
		static GameAssetManager* instance_;

		std::hash_map<std::string, GameAsset<sf::Texture>*> _textures_;
		std::hash_map<sf::Uint32, zinnia::Character*> _trace_characters_;

		//sf::Music _music_;
	};

}