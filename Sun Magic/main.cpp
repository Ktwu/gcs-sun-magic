// Sun Magic.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "game.h"

int main(int argc, char* argv[]) {
	SunMagic::Game* game = new SunMagic::Game();
	game->init();
	game->run();
	game->destroy();
	return 0;
}

