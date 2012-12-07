#pragma once

#include "stdafx.h"
#include "ui/button.h"
#include "ui/style.h"

namespace sun_magic {

	class UiGroup : public Button {
	public:

		UiGroup(float x = 0, float y = 0, float width = 300, float height = 100);
		~UiGroup();

		bool Add(GameObject* object);
		bool Remove(GameObject* object);
		void Clear();
		int Size();
		void UpdateZOrdering();

		GameObject* UpdateFocus(float mouse_x, float mouse_y);

		void Register();
		void Unregister();

		void Update(float elapsed_time);
		void Draw(sf::RenderTarget* target);

		GameObject* operator[](unsigned int i);

	protected:
		std::vector<GameObject*> objects_;
	};

}