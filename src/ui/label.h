#pragma once

#include "stdafx.h"
#include "objects/game_object.h"
#include "ui/button.h"

namespace sun_magic {

	class Label : public Button {
	public:

		Label(float x = 0, float y = 0, float width = 300, float height = 100, sf::String text = "");
		~Label();

	};

}