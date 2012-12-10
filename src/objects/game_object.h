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
		sf::Vector2f GetNegativePosition() {
			return sf::Vector2f(-rect_.left, -rect_.top);
		}
		sf::Vector2f GetSize() {
			return sf::Vector2f(rect_.width, rect_.height);
		}
		sf::FloatRect& GetRect() {
			return rect_;
		}

		void SetSize(sf::Vector2f size) {
			rect_.width = size.x;
			rect_.height = size.y;
		}
		void SetPosition(sf::Vector2f pos) {
			rect_.left = pos.x;
			rect_.top = pos.y;
		}

		float GetZ() {
			return z_;
		}
		virtual void SetZ(float z) {
			z_ = z;
		}

		virtual GameObject* UpdateFocus(float mouse_x, float mouse_y, sf::Vector2i& abs_pos) {
			abs_pos.x += rect_.left;
			abs_pos.y += rect_.top;
			return this;
		}

		virtual void Update(float elapsed_time) = 0;
		virtual void Draw(sf::RenderTarget* target) = 0;
		virtual void Register() = 0;
		virtual void Unregister() = 0;

	protected:
		sf::FloatRect rect_;
		float z_;	// Determines front to back order. Higher z's are in the front
		bool outlined_;
	};

}