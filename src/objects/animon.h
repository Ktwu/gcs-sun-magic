#pragma once

#include "stdafx.h"
#include "events/event.h"
#include "events/gameevent.h"
#include "objects/key_object.h"

namespace sun_magic {

	class Animon : public KeyObject
	{
	public:
		Animon(float x, float y, sf::String texture_name, sf::Color outline, sf::String word);
		~Animon();

		void Update(float elapsed_time);
		bool IsWordWritten(Event event);
		void Grow();

	protected:
		/* Animons have a name, happiness, a timer until they're 'spent', and a sickness bar */
		int level_;
		float happiness_;
		float health_;
		float exist_time_;
		bool done_;
		sf::Clock timer_;
	};
	
}