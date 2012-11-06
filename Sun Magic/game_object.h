#pragma once

#include "stdafx.h"

namespace sun_magic {

	class GameObject {
	public:
		GameObject(float x, float y, float width, float height)
			: _rect(x, y, width, height)
		{ }
		~GameObject() {}

		sf::Vector2f GetPosition() {
			return sf::Vector2f(_rect.left, _rect.top);
		}
		sf::Vector2f GetSize() {
			return sf::Vector2f(_rect.width, _rect.height);
		}
		sf::Rect<float>& GetRect() {
			return _rect;
		}

		int GetZ() {
			return _z;
		}
		void SetZ(int z) {
			_z = z;
		}

		virtual void Update(float elapsed_time) = 0;
		virtual void Draw(sf::RenderTarget* target) = 0;
		virtual void Register() = 0;
		virtual void Unregister() = 0;

	protected:
		sf::Rect<float> _rect;
		int _z;	// Determines front to back order. Higher z's are in the front
	};

}