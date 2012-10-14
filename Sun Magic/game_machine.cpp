#pragma once

#include "stdafx.h"
#include "context.h"
#include "game_machine.h"

#include <cstring>

namespace SunMagic {
	bool GameMachine::Init(SunMagicContext::STATE initialState, GameState* initialGameState) {
		this->_activeState = initialState;
		return this->AddState(initialState, initialGameState);
	}

	SunMagicContext::STATE GameMachine::Update() {

		if (this->_activeState == SunMagicContext::NONE) {
			return this->_activeState;
		}
		
		SunMagicContext::STATE newState = this->_gameStates[this->_activeState]->Update();
		if (newState != SunMagicContext::NONE) {
			this->_activeState = newState;
		}

		return this->_activeState;
	}

	GameState* GameMachine::AddState(SunMagicContext::STATE state, GameState* gameState) {
		if (gameState == NULL) {
			return NULL;
		}

		GameState* oldState = this->_gameStates[state];
		this->_gameStates[state] = gameState;
		return oldState;
	}

	GameState* GameMachine::RemoveState(SunMagicContext::STATE state) {
		GameState* removedState = this->_gameStates[state];
		this->_gameStates[state] = NULL;
		return removedState;
	}

	SunMagicContext::STATE GameMachine::GetActiveStateKey() {
		return this->_activeState;
	}

	GameState* GameMachine::GetActiveState() {
		return this->_gameStates[this->_activeState];
	}
}