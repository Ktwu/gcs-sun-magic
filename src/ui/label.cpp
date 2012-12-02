#include "stdafx.h"
#include "label.h"

#include "assets/gameasset_manager.h"
#include "game.h"
#include "events/event_manager.h"

namespace sun_magic {

	Label::Label(float x, float y, float width, float height, sf::String text) :
		Button(x, y, width, height, text)
	{
		style_.SetAllowHover(false)->SetAllowPress(false);
		style_.SetTextColor(sf::Color::Black);
		style_.SetNormalColor(sf::Color::White);
	}

	Label::~Label() {}

}
