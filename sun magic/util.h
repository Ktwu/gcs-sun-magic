#pragma once

#include "stdafx.h"

namespace sf
{
	// Create a line from p1 to p2
	inline void SetToLine(RectangleShape &rect, Vector2f p1, Vector2f p2, float thickness) {
		sf::Vector2f diff = p2 - p1;
		sf::Vector2f size = Vector2f(sfm::Length(diff), thickness);
		rect.setSize(size);
		rect.setPosition(0.5f * (p1 + p2));
		rect.setOrigin(0.5f * size);
		rect.setRotation(sfm::RADTODEG(atan2(diff.y, diff.x)));
	}
}