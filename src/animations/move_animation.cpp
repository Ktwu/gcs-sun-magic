#pragma once

#include "stdafx.h"
#include "animations/move_animation.h"
#include "events/event.h"
#include "events/event_manager.h"
#include "game.h"
#include "tools/sfm.h"

namespace sun_magic {

	MoveAnimation::MoveAnimation(GameObject* obj, float animate_time) :
		game_object_(obj),
		state_(-1),
		animate_time_(animate_time)
	{
	}

	void MoveAnimation::SetEnd(sf::Vector2f end) {
		end_pos_ = end;
		state_ = 0;
	}

	void MoveAnimation::Update(float elapsed_time) {
		switch (state_) {
		case 0:
			float animate_speed_ = sfm::Length(start_pos_ - end_pos_) / animate_time_;
			sf::Vector2f dist = end_pos_ - game_object_->GetPosition();
			float length = sfm::Length(dist);
			float max_dist = animate_speed_ * elapsed_time;
			if (length > max_dist) {
				dist *= max_dist / length;
			}

			game_object_->GetRect().left += dist.x;
			game_object_->GetRect().top += dist.y;

			if (length < 0.25f) {
				state_ = -1;
				Event e;

				e.source = this;
				e.type = Event::E_GAME_EVENT;
				e.gameEvent = GameEvent::ANIMATION_DONE;
				Game::GetInstance()->GetEventManager()->AddEvent(e);
			}
			break;
		}
	}
}