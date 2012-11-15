#pragma once

#include "stdafx.h"
#include "game.h"

namespace sun_magic {
	namespace tools {

		void ScaleToWindowSize(sf::Sprite& sprite);
		
		sf::String UTF8ToUTF32(const char* utf8str);
	}
}