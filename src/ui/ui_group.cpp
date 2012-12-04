#pragma once

#include "stdafx.h"
#include "events/event_manager.h"
#include "objects/game_object.h"
#include "tools/tools.h"
#include "ui/ui_group.h"

namespace sun_magic {

	UiGroup::UiGroup(float x, float y, float width, float height) :
		Button(x, y, width, height)
	{
		style_.SetAllowHover(false)->SetAllowPress(false);
	}

	UiGroup::~UiGroup() {
		this->objects_.clear();
	}

	bool UiGroup::Add(GameObject* object) {
		if (std::find(objects_.begin(), objects_.end(), object) == objects_.end()) {
			objects_.push_back(object);
			return true;
		}
		return false;
	}
	bool UiGroup::Remove(GameObject* object) {
		std::vector<GameObject*>::iterator i = std::find(objects_.begin(), objects_.end(), object);
		if (i != objects_.end()) {
			objects_.erase(i);
			return true;
		}
		return false;
	}
	void UiGroup::UpdateZOrdering() {
		std::sort(objects_.begin(), objects_.end(), tools::ZSort);
	}

	GameObject* UiGroup::UpdateFocus(float x, float y) {
		// Find all intersecting focus objects
		x -= GetRect().left;
		y -= GetRect().top;

		std::vector<GameObject*> intersections;
		for (std::vector<GameObject*>::iterator focus_iter = objects_.begin(); focus_iter != objects_.end(); focus_iter++) {
			GameObject *object = *focus_iter;
			if (object->GetRect().contains(x, y)) {
				intersections.push_back(object);
			}
		}
		// Sort by z-order
		GameObject *newfocus = NULL;
		if (intersections.size() > 0) {
			std::sort(intersections.begin(), intersections.end(), tools::ZSort);
			newfocus = intersections.front()->UpdateFocus(x, y);
		}

		return (newfocus == NULL) ? this : newfocus;
	}

	void UiGroup::Register() {
		Button::Register();
		EventManager* manager = Game::GetInstance()->GetEventManager();
		for (size_t i = 0; i < objects_.size(); ++i)
			objects_[i]->Register();
	}
	void UiGroup::Unregister() {
		Button::Unregister();
		EventManager* manager = Game::GetInstance()->GetEventManager();
		for (size_t i = 0; i < objects_.size(); ++i)
			objects_[i]->Unregister();
	}

	void UiGroup::Update(float elapsed_time) {
		Button::Update(elapsed_time);
		for (size_t i = 0; i < objects_.size(); ++i)
			objects_[i]->Update(elapsed_time);
	}
	void UiGroup::Draw(sf::RenderTarget* target) {
		Button::Draw(target);
		sf::RenderWindow* window = Game::GetInstance()->GetWindow();
		sf::View view = window->getView();

		for (size_t i = 0; i < objects_.size(); ++i) {
			view.move(objects_[i]->GetNegativePosition());
			window->setView(view);
			objects_[i]->Draw(target);
			view.move(objects_[i]->GetPosition());
		}

		window->setView(view);
	}

}