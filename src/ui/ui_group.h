#pragma once

#include "stdafx.h"
#include "ui/style.h"
#include "ui/ui_element.h"

namespace sun_magic {

	class UiGroup : public UiElement {
	public:

		UiGroup(float x = 0, float y = 0, float width = 300, float height = 100);
		~UiGroup();

		bool UiAdd(GameObject* object);
		bool UiRemove(GameObject* object);
		void UiClear();
		size_t UiSize();
		void UiUpdateZOrdering();

		GameObject* UpdateFocus(int mouse_x, int mouse_y, sf::Vector2f& abs_pos);

		void Register();
		void Unregister();

		void Update(float elapsed_time);
		void Draw(sf::RenderTarget* target);

		GameObject* operator[](unsigned int i);

	protected:
		std::vector<GameObject*> objects_;
	};

}