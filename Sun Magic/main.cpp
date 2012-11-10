// Sun Magic.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "gameasset_manager.h"
#include "game.h"

int main(int argc, char* argv[]) {
	sun_magic::Game* game = sun_magic::Game::GetInstance();
	sun_magic::GameAssetManager* asset_manager = sun_magic::GameAssetManager::GetInstance();

	asset_manager->Init();
	game->Init();

	game->Run();
	game->Destroy();
	return 0;
}

