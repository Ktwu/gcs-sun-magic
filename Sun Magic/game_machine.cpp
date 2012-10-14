#pragma once

#include "stdafx.h"
#include "context.h"
#include "game_machine.h"

#include <cstring>

namespace SunMagic {
	bool GameMachine::Init(SunMagicContext::STATE initialState, GameState* initialGameState) {
		this->activeState = initialState;
		return this->AddState(initialState, initialGameState);
	}
	SunMagicContext::STATE GameMachine::Update() {

		if (this->activeState == SunMagicContext::NONE) {
			return this->activeState;
		}
		
		SunMagicContext::STATE newState = this->gameStates[this->activeState]->Update();
		if (newState != SunMagicContext::NONE) {
			this->activeState = newState;
		}

		return this->activeState;
	}
	GameState* GameMachine::AddState(SunMagicContext::STATE state, GameState* gameState) {
		if (gameState == NULL) {
			return NULL;
		}

		GameState* oldState = this->gameStates[state];
		this->gameStates[state] = gameState;
		return oldState;
	}
	GameState* GameMachine::RemoveState(SunMagicContext::STATE state) {
		GameState* removedState = this->gameStates[state];
		this->gameStates[state] = NULL;
		return removedState;
	}

	SunMagicContext::STATE GetActiveStateKey();
	GameState* GetActiveState();
}