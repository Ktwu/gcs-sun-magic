#pragma once

#include "stdafx.h"
#include "references/hiragana_refs.h"

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

		zinnia::Character* GetTraceCharacter(hiragana::id id);

	private:
		static GameAssetManager* instance_;

		std::map<sf::String, GameAsset<sf::Texture>*> _textures_;
		std::map<hiragana::id, zinnia::Character*> _trace_characters_;

		//sf::Music _music_;
	};

}