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
		border_color = sf::Color::Transparent;
		guide_color = sf::Color::Transparent;
		trace_color = sf::Color::Transparent;
		animate_color = sf::Color::Transparent;
		stroke_color = sf::Color::Transparent;
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

	CharacterTileStyle* CharacterTileStyle::SetBorderColor(sf::Color color) {
		border_color = color;
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

}