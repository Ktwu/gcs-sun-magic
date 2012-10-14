#pragma once

#include "context.h"

namespace SunMagic {

/** @brief The main logic for the game.  Processes states. */
class GameState {

public:
	GameState() {}
	~GameState() {}

	virtual SunMagicContext::STATE Update() = 0;
};

}