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

		sf::Texture* GetTexture(void* key, std::string file);
		void GetTextures(void* key, const std::vector<std::string>& texture_keys, std::vector<sf::Texture*>& textures);
		void ReturnTextures(void* key);
		void CleanUnusedTextures();

		sf::Font* GetFont(void* key, std::string file);
		void GetFonts(void* key, const std::vector<std::string>& font_keys, std::vector<sf::Font*>& fonts);
		void ReturnFonts(void* key);
		void CleanUnusedFonts();

		sf::SoundBuffer* GetSoundBuffer(void* key, std::string file);
		void GetSoundBuffers(void* key, const std::vector<std::string>& soundbuffer_keys, std::vector<sf::SoundBuffer*>& soundbuffers);
		void ReturnSoundBuffers(void* key);
		void CleanUnusedSoundBuffers();

		void GetTraceableCharacters(std::vector<sf::String>& hiraganas);
		zinnia::Character* GetTraceCharacter(sf::String hiragana);

		sf::Sprite GetHiraganaSprite(sf::String hiragana, sf::Texture* sprites);

		int GetHiraganaIndex(sf::String character);
		int GetRomajiIndex(std::string str);
		sf::String HiraganaToRomaji(sf::String hiragana);
		sf::String RomajiToHiragana(sf::String romaji);

	private:
		static GameAssetManager* instance_;

		static sf::Texture* LoadTexture(std::string file) {
			sf::Texture* texture = new sf::Texture();
			if (!texture->loadFromFile(file)) {
				delete texture;
				texture = NULL;
			}
			return texture;
		}
		static sf::Font* LoadFont(std::string file) {
			sf::Font* font = new sf::Font();
			if (!font->loadFromFile(file)) {
				delete font;
				font = NULL;
			}
			return font;
		}
		static sf::SoundBuffer* LoadSoundBuffer(std::string file) {
			sf::SoundBuffer* buffer = new sf::SoundBuffer();
			if (!buffer->loadFromFile(file)) {
				delete buffer;
				buffer = NULL;
			}
			return buffer;
		}

		template<class T>
		void GetAssets(void* key,
			const std::vector<std::string>& asset_keys,
			std::vector<T*>& assets,
			std::hash_map<std::string, GameAsset<T>*> map, std::hash_map<void*, std::vector<std::string>> keymap,
			T* (*loader)(std::string))
		{
			lock_.lock();
			for (size_t i = 0; i < asset_keys.size(); ++i) {
				std::string asset_key = asset_keys[i];
				if (map[asset_key] == NULL) {
					map[asset_key] = new GameAsset<T>();
				}

				if (!map[asset_key]->HasGameAsset()) {
					T* temp = loader(asset_key);
					if (!temp) {
						map.erase(asset_key);
						assets.push_back(NULL);
						continue;
					}

					map[asset_key]->TrySet(temp);
				}

				T* ref = map[asset_key]->GetRef();
				keymap[key].push_back(asset_key);
				assets.push_back(ref);
			}
		    lock_.unlock();
		}

		template<class T>
		T* GetAsset(void* key, std::string file, void (GameAssetManager::*getter)(void*, const std::vector<std::string>&, std::vector<T*>&)) {
			std::vector<T*> assets;
			std::vector<std::string> strings;
			strings.push_back(file);
			((this)->*(getter))(key, strings, assets);
			return assets[0];
		}

		template<class T>
		void ReturnAssets(void* key, std::hash_map<std::string, GameAsset<T>*> map, std::hash_map<void*, std::vector<std::string>> keymap) {
			lock_.lock();
			std::vector<std::string>& strings = keymap[key];
			for (size_t i = 0; i < strings.size(); ++i) {
				if (map[strings[i]] != NULL)
					map[strings[i]]->ReturnRef();
			}
			keymap[key].clear();
			lock_.unlock();
		}

		template<class T>	
		void CleanUnusedAssets(std::hash_map<std::string, GameAsset<T>*> map) {
			lock_.lock();
			std::hash_map<std::string, GameAsset<T>*>::iterator it;
			GameAsset<T>* asset;
			T* resource;

			/* For each resource, try to set its internal value to NULL, which deletes the
				texture we created. */
			for (it = map.begin(); it != map.end(); ++it) {
				asset = it->second;
				resource = asset->TrySet(NULL);
				if (resource != NULL)
					delete resource;
			}
			lock_.unlock();
		}

		sf::Mutex lock_;

		std::hash_map<std::string, GameAsset<sf::Texture>*> textures_;
		std::hash_map<void*, std::vector<std::string>> texture_holders_;

		std::hash_map<std::string, GameAsset<sf::Font>*> fonts_;
		std::hash_map<void*, std::vector<std::string>> font_holders_;

		std::hash_map<std::string, GameAsset<sf::SoundBuffer>*> soundbuffers_;
		std::hash_map<void*, std::vector<std::string>> soundbuffer_holders_;

		std::hash_map<sf::Uint32, zinnia::Character*> trace_characters_;

		std::hash_map<sf::Uint32,int> hiragana_indices_;
		std::hash_map<std::string,int> romaji_indices_;

		sf::Music music;
	};
}