#include <stdafx.h>
#include "ui/character_tile_style.h"

namespace sun_magic {
	CharacterTileStyle::CharacterTileStyle() {
		Clear();
	}

	CharacterTileStyle::~CharacterTileStyle() {
	}

	CharacterTileStyle* CharacterTileStyle::Clear() {
		animation_speed = 0;
		wait_seconds = 0;
		stroke_thickness = 0;
		guide_color = sf::Color(235, 235, 235);
		trace_color = sf::Color(150, 150, 150);
		animate_color = sf::Color(100, 100, 100);
		stroke_color = sf::Color::Black;
		error_bad_color = sf::Color::Red;
		error_good_color = sf::Color::Green;
		return this;
	}

	CharacterTileStyle* CharacterTileStyle::SetAnimationSpeed(float speed) {
		animation_speed = speed;
		return this;
	}

	CharacterTileStyle* CharacterTileStyle::SetAnimationWait(float secondsToWait) {
		wait_seconds = secondsToWait;
		return this;
	}

	CharacterTileStyle* CharacterTileStyle::SetStrokeThickness(float thickness) {
		stroke_thickness = thickness;
		return this;
	}

	CharacterTileStyle* CharacterTileStyle::SetGuideColor(sf::Color color) {
		guide_color = color;
		return this;
	}

	CharacterTileStyle* CharacterTileStyle::SetTraceColor(sf::Color color) {
		trace_color = color;
		return this;
	}

	CharacterTileStyle* CharacterTileStyle::SetAnimateColor(sf::Color color) {
		animate_color = color;
		return this;
	}

	CharacterTileStyle* CharacterTileStyle::SetStrokeColor(sf::Color color) {
		stroke_color = color;
		return this;
	}

	CharacterTileStyle* CharacterTileStyle::SetErrorBadColor(sf::Color color) {
		error_bad_color = color;
		return this;
	}

	CharacterTileStyle* CharacterTileStyle::SetErrorGoodColor(sf::Color color) {
		error_good_color = color;
		return this;
	}

}