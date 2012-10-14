#pragma once

#include <cstring>

#include "context.h"
#include "game_state.h"

namespace SunMagic {

/** @brief The main logic for the game.  Processes states. */
class GameMachine {

public:
	GameMachine() : _activeState(SunMagicContext::NONE) {};
	~GameMachine() {};

	bool Init(SunMagicContext::STATE initialState, GameState* initialGameState);

	SunMagicContext::STATE Update();

	/** @brief Adds a state to the machine, which the machine may then run.
			   May not insert a NULL state or overwrite the state that is
			   currently active.
		@param state The key the state should be mapped to.
		@param gameState The actual state.
		@return Whatever state had previously been mapped to 'state'
	*/
	GameState* AddState(SunMagicContext::STATE state, GameState* gameState);

	/** @brief Tries to remove a state from the machine.  May not remove the
			   currently-active state if it is mapped to the current key.
		@param state The key to the state we're removing.
		@return The state we removed.
	*/
	GameState* RemoveState(SunMagicContext::STATE state);

	SunMagicContext::STATE GetActiveStateKey();
	GameState* GetActiveState();

protected:
	std::map<SunMagicContext::STATE, GameState*> _gameStates;
	SunMagicContext::STATE _activeState;
};

}