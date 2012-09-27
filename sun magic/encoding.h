#include <codecvt>
#include <hash_map>
#include <hash_set>
#include <iostream>
#include <string>
#include <vector>

#include "time.h"

namespace SunMagic {

// Load the list of valid japanese unicode values
std::vector<unsigned short>* GetUnicodeList(char *filename) {
	clock_t start = std::clock();
	std::vector<unsigned short> *chars = new std::vector<unsigned short>();

	// Not all CJK unifed ideographs are valid japanese kanji
	// So we load up a list of only the essential japanese kanji
	std::ifstream fin(filename);
	int j = 0;
	if (fin.is_open()) {
		std::string line;
		std::cout << std::endl;
		// Skip first line
		getline(fin, line);
		while ( fin.good() ) {
			getline(fin, line);
			if (line.size() == 0) continue;

			long l = strtol(line.data(), NULL, 16);
			sf::Uint16 unicode = sf::Uint16((unsigned int)l);
			chars->push_back(unicode);
		}
		std::cout << std::endl;
		fin.close();
	}
	chars->shrink_to_fit();

	clock_t end = std::clock();
	printf("Loaded %d Unicode characters in %.3f seconds.\n",
		chars->size(), (float)(end - start) / CLOCKS_PER_SEC);

	return chars;
}

// Load the mapping of jis to unicode values
std::hash_map<unsigned short, unsigned short>* GetJISToUnicodeMap(char *filename) {
	clock_t start = std::clock();
	std::hash_map<unsigned short, unsigned short> *map =
		new std::hash_map<unsigned short, unsigned short>();

	std::ifstream fin(filename);
	int j = 0;
	if (fin.is_open()) {
		std::string line;
		std::cout << std::endl;
		// Skip first line
		getline(fin, line);
		while ( fin.good() ) {
			getline(fin, line);
			if (line.size() == 0) continue;

			char *end = NULL;
			unsigned long key = strtol(line.data(), &end, 16);
			unsigned long value = strtol(end, NULL, 16);
			map->insert(std::pair<unsigned short, unsigned short>(
				(unsigned short)key, (unsigned short)value));
		}
		std::cout << std::endl;
		fin.close();
	}

	clock_t end = std::clock();
	printf("Loaded %d JIS to Unicode mappings in %.3f seconds.\n",
		map->size(), (float)(end - start) / CLOCKS_PER_SEC);

	return map;
}

unsigned short swapBytes(unsigned short n)
{
	return ((n >> 8) & 0xFF) | (n << 8);
}

// Replaces all JIS codes with unicode codes
void convertJISModelToUnicodeModel(char *jis_filename, char *unicode_filename, char *jis_to_unicode_filename) {
	clock_t start = std::clock();
	printf("Converting JIS model %s to Unicode model %s.\n", jis_filename, unicode_filename);
	std::hash_map<unsigned short, unsigned short>* map = GetJISToUnicodeMap(jis_to_unicode_filename);

	// Open output file
	std::wofstream  fout;
	fout.open(unicode_filename, std::ios::binary);
	fout.imbue(std::locale(fout.getloc(),
		new std::codecvt_utf8<wchar_t, 0x10ffff, std::generate_header>));

	// Open input file
	std::wifstream fin(jis_filename, std::ios::binary);
	// apply BOM-sensitive UTF-16 facet
	fin.imbue(std::locale(fin.getloc(),
		new std::codecvt_utf8<wchar_t, 0x10ffff, std::consume_header>));
	
	int converted = 0;
	int converted_unique = 0;
	int total = 0;
	int total_unique = 0;
	if (fin.is_open()) {
		std::wstring line;
		unsigned short last_char = 0;
		unsigned short character;
		while (fin.good()) {
			wchar_t c = 0;
			while (c != '\n')
				fin.get(c);
			fin.get(c);
			size_t s = sizeof(wchar_t);

			//getline(fin, line);
			//if (line.size() == 0) continue;
			total++;
			continue;

			size_t index = line.find(' ');
			if (index > 1) {
				unsigned char a[] = {line.data()[0], line.data()[1], 0};
				wchar_t c[] = {line.data()[0], 0};
				character = swapBytes(*((unsigned short*)line.data()));
				std::hash_map<unsigned short, unsigned short>::iterator it = map->find(character);
				if (it != map->end()) {
					unsigned short unicode[] = { it->second, 0 };
					unsigned char a[] = { ((unsigned char *)unicode)[1], ((unsigned char *)unicode)[0], 0 };
					wchar_t b[] = { unicode[0], 0 };
					if (last_char != character) {
						converted_unique++;
						total_unique++;
						//std::cout << std::hex << character << "->" << std::hex << unicode << ", ";
						last_char = character;
					}
					converted++;
					fout << a << line.substr(2) << std::endl;
					continue;
				}
			}
			character = (line.front() >> 8) & 0xFF;
			if (last_char != character) {
				total_unique++;
				last_char = character;
			}
			fout << line << std::endl;
		}
		fin.close();
	}
	fout.close();

	delete map;
	clock_t end = std::clock();
	printf("\nConverted %d of %d characters and %d of %d unique characters in %.3f seconds.\n",
		converted, total, converted_unique, total_unique, (float)(end - start) / CLOCKS_PER_SEC);
}

}