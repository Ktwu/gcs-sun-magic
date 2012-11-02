#pragma once

#include "stdafx.h"
#include "gameasset.h"

namespace sun_magic {

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
		sf::Texture* GetTexture(sf::String textureName);
		void ReturnTexture(sf::String textureName);
		void CleanUnusedTextures();

	private:
		static GameAssetManager* instance_;

		std::map<sf::String, GameAsset<sf::Texture>*> _textures_;
		//sf::Music _music_;
	};
}