#pragma once

#include "stdafx.h"
#include "machine_state.h"

namespace SunMagic {

	/** @brief The main logic for the game.  Processes states. */
	template <class T>
	class Machine {
	public:
		Machine() {};
		~Machine() {};

		bool init(T initialState, MachineState<T>* initialGameState) {
			this->_activeState = initialState;
			return this->addState(initialState, initialGameState);
		}

		MachineState<T>* addState(T state, MachineState<T>* gameState) {
			if (gameState == NULL) {
				return NULL;
			}

			MachineState<T>* oldState = this->_gameStates[state];
			this->_gameStates[state] = gameState;
			return oldState;
		}

		MachineState<T>* removeState(T state) {
			MachineState<T>* removedState = this->_gameStates[state];
			this->_gameStates[state] = NULL;
			return removedState;
		}
		
		MachineState<T>* getActiveState() {
			return this->_gameStates[this->_activeState];
		}

		T getActiveStateKey() {
			return this->_activeState;
		}

		T update() {
			if (this->_gameStates[this->_activeState] == NULL) {
				return this->_activeState;
			}
		
			T newState = this->_gameStates[this->_activeState]->update();
			if (this->_gameStates[newState] != NULL) {
				this->_activeState = newState;
			}

			return this->_activeState;
		}

	protected:
		std::map<T, MachineState<T>*> _gameStates;
		T _activeState;
	};

}