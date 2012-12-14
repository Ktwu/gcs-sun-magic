#pragma once

#include "stdafx.h"
#include "ui/style.h"

namespace sun_magic {

	class CharacterTileStyle {
	public:
		CharacterTileStyle();
		~CharacterTileStyle();

		CharacterTileStyle* Clear();

		CharacterTileStyle* SetAnimationSpeed(float speed);
		CharacterTileStyle* SetAnimationWait(float secondsToWait);

		CharacterTileStyle* SetStrokeThickness(float thickness);

		CharacterTileStyle* SetBorderColor(sf::Color color);
		CharacterTileStyle* SetGuideColor(sf::Color color);
		CharacterTileStyle* SetTraceColor(sf::Color color);
		CharacterTileStyle* SetAnimateColor(sf::Color color);
		CharacterTileStyle* SetStrokeColor(sf::Color color);
		CharacterTileStyle* SetErrorBadColor(sf::Color color);
		CharacterTileStyle* SetErrorGoodColor(sf::Color color);

		float animation_speed;
		float wait_seconds;

		float stroke_thickness;

		sf::Color border_color;
		sf::Color guide_color;
		sf::Color trace_color;
		sf::Color animate_color;
		sf::Color stroke_color;
		sf::Color error_bad_color;
		sf::Color error_good_color;

	};

}