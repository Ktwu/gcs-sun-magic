// Animon.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "game.h"

int main(int argc, char* argv[]) {
	sun_magic::Game* game = sun_magic::Game::GetInstance();
	game->Init();

	game->Run();
	game->Destroy();
	return 0;
}

