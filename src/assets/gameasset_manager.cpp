#include "stdafx.h"
#include "gameasset_manager.h"

#include "assets/gameasset.h"
#include "references/refs.h"
#include "tools/tools.h"

namespace sun_magic {

	GameAssetManager* GameAssetManager::instance_ = NULL;

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
	}

	sf::Texture* GameAssetManager::GetTexture(std::string texture_name) {
		if (textures_[texture_name] == NULL) {
			textures_[texture_name] = new GameAsset<sf::Texture>();
		}

		if (!textures_[texture_name]->HasGameAsset()) {
			sf::Texture* temp = new sf::Texture();
			if (!temp->loadFromFile(texture_name)) {
				textures_.erase(texture_name);
				return NULL;
			}

			textures_[texture_name]->TrySet(temp);
		}

		return textures_[texture_name]->GetRef();
	}

	void GameAssetManager::ReturnTexture(std::string texture_name) {
		if (textures_[texture_name] != NULL)
			textures_[texture_name]->ReturnRef();
	}


	void GameAssetManager::CleanUnusedTextures() {
		std::hash_map<std::string, GameAsset<sf::Texture>*>::iterator it;
		GameAsset<sf::Texture>* resource;
		sf::Texture* texture;

		/* For each resource, try to set its internal value to NULL, which deletes the
			texture we created. */
		for (it = textures_.begin(); it != textures_.end(); ++it) {
			resource = it->second;
			texture = resource->TrySet(NULL);
			if (texture != NULL)
				delete texture;
		}
	}

	sf::Font* GameAssetManager::GetFont(std::string font_name) {
		if (fonts_[font_name] == NULL) {
			fonts_[font_name] = new GameAsset<sf::Font>();
		}

		if (!fonts_[font_name]->HasGameAsset()) {
			sf::Font* temp = new sf::Font();
			if (!temp->loadFromFile(font_name)) {
				textures_.erase(font_name);
				return NULL;
			}

			fonts_[font_name]->TrySet(temp);
		}

		return fonts_[font_name]->GetRef();
	}

	void GameAssetManager::ReturnFont(std::string font_name) {
		if (fonts_[font_name] != NULL)
			fonts_[font_name]->ReturnRef();
	}
	void GameAssetManager::CleanUnusedFonts() {
		std::hash_map<std::string, GameAsset<sf::Font>*>::iterator it;
		GameAsset<sf::Font>* resource;
		sf::Font* font;

		/* For each resource, try to set its internal value to NULL, which deletes the
			texture we created. */
		for (it = fonts_.begin(); it != fonts_.end(); ++it) {
			resource = it->second;
			font = resource->TrySet(NULL);
			if (font != NULL)
				delete font;
		}
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

}