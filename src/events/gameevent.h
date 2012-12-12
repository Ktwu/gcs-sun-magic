#pragma once

#include "stdafx.h"

namespace sun_magic {

	enum GameEvent {
		DEFAULT,
		ANIMATION_DONE,

		// Test
		KEY_CLICK,
		ANIMON_LEVEL_UP,

		ANIMON_RIGHT,
		ANIMON_WRONG,

		// Final count for use by the game
		NUM_EVENTS
	};

}