#include "stdafx.h"
#include "gameasset_manager.h"

#include "assets/gameasset.h"
#include "references/refs.h"
#include "tools/tools.h"

namespace sun_magic {

	GameAssetManager* GameAssetManager::instance_ = NULL;

	sf::String GameAssetManager::hiragana_strings[] = {
		L"‚ ", L"‚¢", L"‚¤", L"‚¦", L"‚¨",
		L"‚©", L"‚«", L"‚­", L"‚¯", L"‚±",
		L"‚³", L"‚µ", L"‚·", L"‚¹", L"‚»",
		L"‚½", L"‚¿", L"‚Â", L"‚Ä", L"‚Æ",
		L"‚È", L"‚É", L"‚Ê", L"‚Ë", L"‚Ì",
		L"‚Í", L"‚Ð", L"‚Ó", L"‚Ö", L"‚Ù",
		L"‚Ü", L"‚Ý", L"‚Þ", L"‚ß", L"‚à",
		L"‚â", L"‚ä", L"‚æ",
		L"‚ç", L"‚è", L"‚é", L"‚ê", L"‚ë",
		L"‚í", L"‚ð",
		L"‚ñ"
	};
	std::string GameAssetManager::romaji_strings[] = {
		"a", "i", "u", "e", "o",
		"ka", "ki", "ku", "ke", "ko",
		"sa", "shi", "su", "se", "so",
		"ta", "chi", "tsu", "te", "to",
		"na", "ni", "nu", "ne", "no",
		"ha", "hi", "fu", "he", "ho",
		"ma", "mi", "mu", "me", "mo",
		"ya", "yu", "yo",
		"ra", "ri", "ru", "re", "ro",
		"wa", "wo",
		"n"
	};
	sf::Color GameAssetManager::symbols_colors[] = {
		tools::HSVColor(0/23.f,1,1), tools::HSVColor(5/23.f,0.7f,0.7f), tools::HSVColor(10/23.f,1,1), tools::HSVColor(15/23.f,0.7f,0.7f), tools::HSVColor(20/23.f,1,1),
		tools::HSVColor(2/23.f,0.7f,0.7f), tools::HSVColor(7/23.f,1,1), tools::HSVColor(12/23.f,0.7f,0.7f), tools::HSVColor(17/23.f,1,1), tools::HSVColor(22/23.f,0.7f,0.7f),
		tools::HSVColor(4/23.f,1,1), tools::HSVColor(9/23.f,0.7f,0.7f), tools::HSVColor(14/23.f,1,1), tools::HSVColor(19/23.f,0.7f,0.7f), tools::HSVColor(1/23.f,1,1),
		tools::HSVColor(6/23.f,0.7f,0.7f), tools::HSVColor(11/23.f,1,1), tools::HSVColor(16/23.f,0.7f,0.7f), tools::HSVColor(21/23.f,1,1), tools::HSVColor(3/23.f,0.7f,0.7f),
		tools::HSVColor(8/23.f,1,1), tools::HSVColor(13/23.f,0.7f,0.7f), tools::HSVColor(18/23.f,1,1), tools::HSVColor(0.f,0.7f,0.7f), tools::HSVColor(5/23.f,1,1),
		tools::HSVColor(10/23.f,0.7f,0.7f), tools::HSVColor(15/23.f,1,1), tools::HSVColor(20/23.f,0.7f,0.7f), tools::HSVColor(2/23.f,1,1), tools::HSVColor(7/23.f,0.7f,0.7f),
		tools::HSVColor(12/23.f,1,1), tools::HSVColor(17/23.f,0.7f,0.7f), tools::HSVColor(22/23.f,1,1), tools::HSVColor(4/23.f,0.7f,0.7f), tools::HSVColor(9/23.f,1,1),
		tools::HSVColor(14/23.f,0.7f,0.7f), tools::HSVColor(19/23.f,1,1), tools::HSVColor(1/23.f,0.7f,0.7f),
		tools::HSVColor(6/23.f,1,1), tools::HSVColor(11/23.f,0.7f,0.7f), tools::HSVColor(16/23.f,1,1), tools::HSVColor(21/23.f,0.7f,0.7f), tools::HSVColor(3/23.f,1,1),
		tools::HSVColor(8/23.f,0.7f,0.7f), tools::HSVColor(13/23.f,1,1),
		tools::HSVColor(18/23.f,0.7f,0.7f)
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

	int GameAssetManager::GetHiraganaIndex(sf::Uint32 character) {
		std::hash_map<sf::Uint32,int>::iterator iter = hiragana_indices_.find(character);
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