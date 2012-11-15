#include "stdafx.h"
#include "gameasset_manager.h"

#include "assets/gameasset.h"
#include "references/file_refs.h"
#include "tools/tools.h"

namespace sun_magic {

	GameAssetManager* GameAssetManager::instance_ = NULL;

	void GameAssetManager::Init() {
		std::ifstream trace_characters;
		trace_characters.open(file_refs::HIRAGANA_TRACE);

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
					_trace_characters_[utf32_str[0]] = character;
				else
					std::cout << "Line " << line << " Error: " << parsed_line << std::endl;
			}
			line++;
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
		std::hash_map<std::string, GameAsset<sf::Texture>*>::iterator it;
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

	void GameAssetManager::GetTraceableCharacters(std::vector<sf::Uint32>& characters) {
		for (std::hash_map<sf::Uint32, zinnia::Character*>::iterator iter =
				_trace_characters_.begin(); iter != _trace_characters_.end(); iter++) {
			characters.push_back(iter->first);
		}
	}

	zinnia::Character* GameAssetManager::GetTraceCharacter(sf::Uint32 utf32_character) {
		return _trace_characters_[utf32_character];
	}

}