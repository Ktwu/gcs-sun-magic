#pragma once

#include "stdafx.h"
#include "gameasset_manager.h"

namespace sun_magic {

	GameAssetManager* GameAssetManager::instance_ = NULL;

	void GameAssetManager::Init() {}

	sf::Texture* GameAssetManager::GetTexture(sf::String texture_name) {
		if (_textures_[texture_name] == NULL) {
			_textures_[texture_name] = new GameAsset<sf::Texture>();
		}

		if (!_textures_[texture_name]->HasGameAsset()) {
			sf::Texture* temp = new sf::Texture();
			if (!temp->loadFromFile(texture_name)) {
				_textures_.erase(texture_name);
				return NULL;
			}

			_textures_[texture_name]->TrySet(temp);
		}

		return _textures_[texture_name]->GetRef();
	}

	void GameAssetManager::ReturnTexture(sf::String texture_name) {
		if (_textures_[texture_name] != NULL)
			_textures_[texture_name]->ReturnRef();
	}


	void GameAssetManager::CleanUnusedTextures() {
		std::map<sf::String, GameAsset<sf::Texture>*>::iterator it;
		GameAsset<sf::Texture>* resource;
		sf::Texture* texture;

		/* For each resource, try to set its internal value to NULL, which deletes the
		   texture we created. */
		for (it = _textures_.begin(); it != _textures_.end(); ++it) {
			resource = it->second;
			texture = resource->TrySet(NULL);
			if (texture != NULL)
				delete texture;
		}
	}
}