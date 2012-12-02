#pragma once

#include "stdafx.h"
#include "assets/gameasset_manager.h"
#include "events/event.h"
#include "game.h"
#include "objects/key_object.h"
#include "references/refs.h"

namespace sun_magic {
	namespace event_callbacks {

		void Cat(KeyObject* cat, Event event) {
			if (event.message != cat->GetWord())
				return;

			Game::GetInstance()->GetDictionary()->AddWord(
				cat->GetWord(),
				GameAssetManager::GetInstance()->GetTexture(refs::textures::objects::NEKO));

			GameAssetManager* manager = GameAssetManager::GetInstance();
			manager->ReturnTexture(refs::textures::objects::NEKO);
			cat->SetTexture(*manager->GetTexture(refs::textures::objects::NEKO_HAPPY));
			cat->SetEventCallback(NULL);
		}

		void Love(KeyObject* love, Event event) {
			if (event.message != love->GetWord())
				return;

			Game::GetInstance()->GetDictionary()->AddWord(
				love->GetWord(),
				GameAssetManager::GetInstance()->GetTexture(refs::textures::objects::STUPID_LOVE));
			love->SetVisible(true);
			love->SetEventCallback(NULL);
		}

	}
}