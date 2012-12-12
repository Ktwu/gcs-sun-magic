#pragma once

#include "stdafx.h"
#include "events/event.h"

namespace sun_magic {

	class Animation : public EventListener	{
	public:
		Animation() {}
		~Animation() {}

		void Update(float elapsed_time) {}
		void Draw(sf::RenderTarget* target) {}

		void ProcessEvent(Event event) {}
	};
	
}