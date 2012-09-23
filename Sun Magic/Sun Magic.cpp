// Sun Magic.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "Game.h"

// TODO delete this once everyone's verified it works
void zinniaTest() {
	zinnia::Recognizer *recognizer = zinnia::Recognizer::create();
	if (!recognizer->open("zinnia/models/handwriting-ja.model")) {
		std::cout << "ERROR: " << recognizer->what() << std::endl;
		return;
	}

	zinnia::Character *character = zinnia::Character::create();
	character->clear();
	character->set_width(300);
	character->set_height(300);
	character->add(0, 51, 29);
	character->add(0, 117, 41);
	character->add(1, 99, 65);
	character->add(1, 219, 77);
	character->add(2, 27, 131);
	character->add(2, 261, 131);
	character->add(3, 129, 17);
	character->add(3, 57, 203);
	character->add(4, 111, 71);
	character->add(4, 219, 173);
	character->add(5, 81, 161);
	character->add(5, 93, 281);
	character->add(6, 99, 167);
	character->add(6, 207, 167);
	character->add(6, 189, 245);
	character->add(7, 99, 227);
	character->add(7, 189, 227);
	character->add(8, 111, 257);
	character->add(8, 189, 245);

	zinnia::Result *result = recognizer->classify(*character, 10);
	if (!result) {
		std::cout << "ERROR: " << recognizer->what() << std::endl;
		return;
	}
	for (size_t i = 0; i < result->size(); ++i) {
		std::cout << result->value(i) << "\t" << result->score(i) << std::endl;
	}
	delete result;

	delete character;
	delete recognizer;
}


int main(int argc, char* argv[]) {
	// First, check to make sure we can use Zinnia.
	zinniaTest();

	Game::Start();
	return 0;
}

