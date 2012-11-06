#pragma once

#include "stdafx.h"

namespace sun_magic {

	class GameObject : sf::Drawable {
	public:
		GameObject(float x, float y)
			: _position(sf::Vector2f(x, y)) { }
		~GameObject() {}

		sf::Vector2f GetPosition() {
			return _position;
		}

		void SetPosition(sf::Vector2f pos) {
			_position = pos;
		}

		virtual void Update() = 0;
		virtual void draw(sf::RenderTarget& target, sf::RenderStates state) const = 0;

	protected:
		sf::Vector2f _position;
	};

}