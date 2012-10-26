#pragma once

#include "stdafx.h"

namespace SunMagic {

	class GameObject {

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

		virtual void Update(float elapsedSeconds) = 0;
		virtual void Draw(sf::RenderWindow *mainWindow) = 0;

	protected:
		sf::Vector2f _position;
	};

}