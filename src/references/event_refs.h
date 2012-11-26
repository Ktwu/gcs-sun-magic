#pragma once

#include "stdafx.h"
#include "assets/gameasset_manager.h"
#include "events/event.h"
#include "game.h"
#include "objects/key_object.h"
#include "references/texture_refs.h"

namespace sun_magic {
	namespace event_callbacks {

		void Cat(KeyObject* cat, Event event) {
			if (event.message != cat->GetWord())
				return;

			Game::GetInstance()->GetDictionary()->AddWord(
				sf::String(L"ねこ"),
				GameAssetManager::GetInstance()->GetTexture(textures::objects::NEKO));

			GameAssetManager* manager = GameAssetManager::GetInstance();
			manager->ReturnTexture(textures::objects::NEKO);
			cat->SetTexture(*manager->GetTexture(textures::objects::NEKO_HAPPY));
			cat->SetEventCallback(NULL);
		}

		void Love(KeyObject* love, Event event) {
			if (event.message != love->GetWord())
				return;
			std::cout << "LOVE\n";
			love->SetVisible(true);
			love->SetEventCallback(NULL);
		}

	}
}