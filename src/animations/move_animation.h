#pragma once

#include "stdafx.h"
#include "animations/animation.h"
#include "events/event.h"
#include "objects/game_object.h"

namespace sun_magic {

	class MoveAnimation : public Animation	{
	public:
		MoveAnimation(GameObject* obj, float animate_time);
		~MoveAnimation() {}

		void SetEnd(sf::Vector2f end);
		void Update(float elapsed_time);

	private:
		GameObject* game_object_;

		sf::Vector2f total_delta;
		sf::Vector2f start_pos_;
		sf::Vector2f end_pos_;

		float animate_time_;
		int state_;
	};
	
}