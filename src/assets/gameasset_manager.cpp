#include "stdafx.h"
#include "gameasset_manager.h"

#include "assets/gameasset.h"
#include "references/refs.h"
#include "tools/tools.h"

namespace sun_magic {

	GameAssetManager* GameAssetManager::instance_ = NULL;

	sf::String GameAssetManager::hiragana_strings[] = {
		L"あ", L"い", L"う", L"え", L"お",
		L"か", L"き", L"く", L"け", L"こ",
		L"さ", L"し", L"す", L"せ", L"そ",
		L"た", L"ち", L"つ", L"て", L"と",
		L"な", L"に", L"ぬ", L"ね", L"の",
		L"は", L"ひ", L"ふ", L"へ", L"ほ",
		L"ま", L"み", L"む", L"め", L"も",
		L"や", L"ゆ", L"よ",
		L"ら", L"り", L"る", L"れ", L"ろ",
		L"わ", L"を",
		L"ん"
	};
	std::string GameAssetManager::romaji_strings[] = {
		"a",  "i",   "u",   "e",  "o",
		"ka", "ki",  "ku",  "ke", "ko",
		"sa", "shi", "su",  "se", "so",
		"ta", "chi", "tsu", "te", "to",
		"na", "ni",  "nu",  "ne", "no",
		"ha", "hi",  "fu",  "he", "ho",
		"ma", "mi",  "mu",  "me", "mo",
		"ya",        "yu",  "yo",
		"ra", "ri",  "ru",  "re", "ro",
		"wa",                     "wo",
		"n"
	};
	sf::String GameAssetManager::about_strings[] = {
		L"Nickname: あ(a)\nあひる(ahiru) the fluffy duck", 
		L"Nickname: い(i)\nいぬ(inu) the loyal dog",
		L"Nickname: う(u)\nうさぎ(usagi) the quick rabbit",
		L"Nickname: え(e)\nえび(ebi) the delicious shrimp",
		L"Nickname: お(o)\nおばけ(obake) the lonely ghost",
		L"Nickname: か(ka)\nかみ(kami) the slow turtle",
		L"Nickname: き(ki)\nきつね(kitsune) the sly fox", 
		L"Nickname: く(ku)\nくも(kumo) the huge bear", 
		L"Nickname: け(ke)\nけーき(keeki) the legendary dessert",
		L"Nickname: こ(ko)\nこあら(koara) the adorable koala",
		L"Nickname: さ(sa)\nさかな(sakana) the goofy fish",
		L"Nickname: し(shi)\nしし(shishi) the brave lion",
		L"Nickname: す(su)\nすいか(suika) the seedy watermelon",
		L"Nickname: せ(se)\nせんす(sensu) the beautiful folding fan",
		L"Nickname: そ(so)\nそふとくりーむ(sofuto kuriimu) the sweet soft ice cream",
		L"Nickname: た(ta)\nたこ(tako) the flexible octopus",
		L"Nickname: ち(chi)\nちきん(chikin) the tasty chicken",
		L"Nickname: つ(tsu)\nつる(tsuru) the elegant crane",
		L"Nickname: て(te)\nてんし(tenshi) the holy angel",
		L"Nickname: と(to)\nとら(tora) the fierce tiger",
		L"Nickname: な(na)\nなまけもの(namakemono) the patient sloth",
		L"Nickname: に(ni)\nにじ(niji) the vibrant rainbow",
		L"Nickname: ぬ(nu)\nぬいばり(nuibari) the precise sewing needle",
		L"Nickname: ね(ne)\nねこ(neko) the curious cat",
		L"Nickname: の(no)\nのみもの(nomimono) the refreshing drink",
		L"Nickname: は(ha)\nはな(hana) the delicate flower",
		L"Nickname: ひ(hi)\nひつじ(hitsuji) the poofy sheep",
		L"Nickname: ふ(fu)\nふくろう(fukurou) the wise owl",
		L"Nickname: へ(he)\nへび(hebi) the devious snake",
		L"Nickname: ほ(ho)\nほたる(hotaru) the glowing firefly",
		L"Nickname: ま(ma)\nまつむし(matsumushi) the tiny pine cricket",
		L"Nickname: み(mi)\nみるく(miruku) the energizing milk",
		L"Nickname: む(mu)\nむーす(muusu) the proud moose",
		L"Nickname: め(me) the horrifying eyeball",
		L"Nickname: も(mo)\nもも(momo) the plump peach",
		L"Nickname: や(ya)\nやぎ(yagi) the hardy goat",
		L"Nickname: ゆ(yu)\nゆきだるま(yukidaruma) the chilly snowman",
		L"Nickname: よ(yo)\nよっと(yotto) the swift yacht",
		L"Nickname: ら(ra)\nらくだ(rakuda) the enduring camel",
		L"Nickname: り(ri)\nりす(risu) the hyper squirrel",
		L"Nickname: る(ru)\nるび(rubi) the valuable ruby",
		L"Nickname: れ(re)\nれもん(remon) the sour lemon",
		L"Nickname: そ(ro)\nそうび(roubi) the thorned rose",
		L"Nickname: わ(wa)\nわに(wani) the grinning crocodile",
		L"Nickname: を(wo) the sentence component",
		L"Nickname: ん(n) the letter N"
	};
	sf::Color GameAssetManager::symbols_colors[] = {
		tools::HSVColor(0/46.f,1,1),  tools::HSVColor(7/46.f,1,1),	tools::HSVColor(14/46.f,1,1), tools::HSVColor(21/46.f,1,1),	tools::HSVColor(28/46.f,1,1),
		tools::HSVColor(35/46.f,1,1), tools::HSVColor(42/46.f,1,1),	tools::HSVColor(3/46.f,1,1),  tools::HSVColor(10/46.f,1,1),	tools::HSVColor(17/46.f,1,1),
		tools::HSVColor(24/46.f,1,1), tools::HSVColor(31/46.f,1,1),	tools::HSVColor(38/46.f,1,1), tools::HSVColor(45/46.f,1,1),	tools::HSVColor(6/46.f,1,1),
		tools::HSVColor(13/46.f,1,1), tools::HSVColor(20/46.f,1,1),	tools::HSVColor(27/46.f,1,1), tools::HSVColor(34/46.f,1,1),	tools::HSVColor(41/46.f,1,1),
		tools::HSVColor(2/46.f,1,1),  tools::HSVColor(9/46.f,1,1),	tools::HSVColor(16/46.f,1,1), tools::HSVColor(23/46.f,1,1),	tools::HSVColor(30/46.f,1,1),
		tools::HSVColor(37/46.f,1,1), tools::HSVColor(44/46.f,1,1),	tools::HSVColor(5/46.f,1,1),  tools::HSVColor(12/46.f,1,1), tools::HSVColor(19/46.f,1,1),
		tools::HSVColor(26/46.f,1,1), tools::HSVColor(33/46.f,1,1),	tools::HSVColor(40/46.f,1,1), tools::HSVColor(1/46.f,1,1),  tools::HSVColor(8/46.f,1,1),
		tools::HSVColor(15/46.f,1,1),								tools::HSVColor(22/46.f,1,1),								tools::HSVColor(29/46.f,1,1),
		tools::HSVColor(36/46.f,1,1), tools::HSVColor(43/46.f,1,1),	tools::HSVColor(4/46.f,1,1), tools::HSVColor(11/46.f,1,1),	tools::HSVColor(18/46.f,1,1),
		tools::HSVColor(25/46.f,1,1),																							tools::HSVColor(32/46.f,1,1),
		tools::HSVColor(39/46.f,1,1)
	};
	int GameAssetManager::symbols_pos[] = {
			0,  1,  2,  3,  4,
			5,  6,  7,  8,  9,
			10, 11, 12, 13, 14,
			15, 16, 17, 18, 19,
			20, 21, 22, 23, 24,
			25, 26, 27, 28, 29,
			30, 31, 32, 33, 34,
			35,     37,     39,
			40, 41, 42, 43, 44,
			45,             49,
			50
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

	// Textures
	sf::Texture* GameAssetManager::GetTexture(void* key, std::string file) {
		return GetAsset(key, file, &GameAssetManager::GetTextures);
	}
	void GameAssetManager::GetTextures(void* key, const std::vector<std::string>& texture_keys, std::vector<sf::Texture*>& textures) {
		GetAssets(key, texture_keys, textures, this->textures_, this->texture_holders_, this->LoadTexture);
	}
	void GameAssetManager::ReturnTextures(void* key) {
		ReturnAssets(key, this->textures_, this->texture_holders_);
	}
	void GameAssetManager::CleanUnusedTextures() {
		CleanUnusedAssets(textures_);
	}

	// Fonts
	sf::Font* GameAssetManager::GetFont(void* key, std::string file) {
		return GetAsset(key, file, &GameAssetManager::GetFonts);
	}
	void GameAssetManager::GetFonts(void* key, const std::vector<std::string>& font_keys, std::vector<sf::Font*>& fonts) {
		GetAssets(key, font_keys, fonts, this->fonts_, this->font_holders_, this->LoadFont);
	}
	void GameAssetManager::ReturnFonts(void* key) {
		ReturnAssets(key, this->fonts_, this->font_holders_);
	}
	void GameAssetManager::CleanUnusedFonts() {
		CleanUnusedAssets(fonts_);
	}

	// Sound Buffers
	sf::SoundBuffer* GameAssetManager::GetSoundBuffer(void* key, std::string file) {
		return GetAsset(key, file, &GameAssetManager::GetSoundBuffers);
	}
    void GameAssetManager::GetSoundBuffers(void* key, const std::vector<std::string>& soundbuffer_keys, std::vector<sf::SoundBuffer*>& soundbuffers) {
		GetAssets(key, soundbuffer_keys, soundbuffers, this->soundbuffers_, this->soundbuffer_holders_, this->LoadSoundBuffer);
	}
	void GameAssetManager::ReturnSoundBuffers(void* key) {
		ReturnAssets(key, this->soundbuffers_, this->soundbuffer_holders_);
	}
	void GameAssetManager::CleanUnusedSoundBuffers() {
		CleanUnusedAssets(soundbuffers_);
	}

	void GameAssetManager::GetTraceableCharacters(std::vector<sf::String>& hiraganas) {
		for (std::hash_map<sf::Uint32, zinnia::Character*>::iterator iter =
				trace_characters_.begin(); iter != trace_characters_.end(); iter++) {
			hiraganas.push_back(sf::String(iter->first));
		}
	}

	zinnia::Character* GameAssetManager::GetTraceCharacter(sf::String hiragana) {
		return trace_characters_[hiragana[0]];
	}

	sf::Sprite GameAssetManager::GetHiraganaSprite(sf::String hiragana, sf::Texture* sprites) {
		int index = GetHiraganaIndex(hiragana);
		int pos = symbols_pos[index];

		sf::Vector2u size = sprites->getSize();
		size.x /= ROW_SIZE;
		size.y /= COL_SIZE;

		int x = pos % ROW_SIZE;
		int y = pos / ROW_SIZE;

		return sf::Sprite(*sprites, sf::IntRect(size.x * x, size.y * y, size.x, size.y));
	}

	int GameAssetManager::GetHiraganaIndex(sf::String character) {
		std::hash_map<sf::Uint32,int>::iterator iter = hiragana_indices_.find(character[0]);
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