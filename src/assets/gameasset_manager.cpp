#include "stdafx.h"
#include "gameasset_manager.h"

#include "assets/gameasset.h"
#include "references/file_refs.h"

namespace sun_magic {

	GameAssetManager* GameAssetManager::instance_ = NULL;

	void GameAssetManager::Init() {
		std::ifstream trace_characters;
		trace_characters.open(file_refs::HIRAGANA_TRACE);

		// We need to parse our trace characters out
		zinnia::Character* character;
		std::string parsed_line;
		hiragana::id index = hiragana::A;
		while (!trace_characters.eof()) {
			parsed_line.clear();
			std::getline(trace_characters, parsed_line);
			character = zinnia::createCharacter();
			character->parse(parsed_line.c_str());
			index = (hiragana::id) atoi(character->value());
			_trace_characters_[index] = character;
		}
	}

	sf::Texture* GameAssetManager::GetTexture(std::string texture_name) {
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

	void GameAssetManager::ReturnTexture(std::string texture_name) {
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

	zinnia::Character* GameAssetManager::GetTraceCharacter(hiragana::id id) {
		return _trace_characters_[id];
	}

}