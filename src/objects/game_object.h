#pragma once

#include "stdafx.h"

namespace sun_magic {

	class GameObject {
	public:
		GameObject(float x, float y, float width, float height)
			: rect_(x, y, width, height)
		{ }
		~GameObject() {}

		sf::Vector2f GetPosition() {
			return sf::Vector2f(rect_.left, rect_.top);
		}
		sf::Vector2f GetSize() {
			return sf::Vector2f(rect_.width, rect_.height);
		}
		sf::Rect<float>& GetRect() {
			return rect_;
		}

		int GetZ() {
			return z_;
		}
		void SetZ(int z) {
			z_ = z;
		}

		virtual void Update(float elapsed_time) = 0;
		virtual void Draw(sf::RenderTarget* target) = 0;
		virtual void Register() = 0;
		virtual void Unregister() = 0;

	protected:
		sf::Rect<float> rect_;
		int z_;	// Determines front to back order. Higher z's are in the front
		bool outlined_;
	};

}