#pragma once

#include "stdafx.h"
#include "machine_state.h"

namespace sun_magic {

	/** @brief The main logic for the game.  Processes states. */
	template <class T>
	class Machine {
	public:
		Machine() {};
		~Machine() {};

		bool Init(T initial_state, MachineState<T>* initial_game_state);
		MachineState<T>* AddState(T state, MachineState<T>* game_state);
		MachineState<T>* RemoveState(T state);
		MachineState<T>* GetActiveState();
		T GetActiveStateKey();
		T Update(float elapsed_time);

	protected:
		std::map<T, MachineState<T>*> _game_states_;
		T _active_state_;
	};


	template <class T>
	bool Machine<T>::Init(T initial_state, MachineState<T>* initial_game_state) {
		this->_active_state_ = initial_state;
		this->AddState(initial_state, initial_game_state);
		initial_game_state->RegisterState(NULL);
		return true;
	}

	template <class T>
	MachineState<T>* Machine<T>::AddState(T state, MachineState<T>* game_state) {
		if (game_state == NULL) {
			return NULL;
		}

		MachineState<T>* old_state = this->_game_states_[state];
		this->_game_states_[state] = game_state;
		return old_state;
	}

	template <class T>
	MachineState<T>* Machine<T>::RemoveState(T state) {
		MachineState<T>* removed_state = this->_game_states_[state];
		this->_game_states_[state] = NULL;
		return removed_state;
	}
	
	template <class T>
	MachineState<T>*  Machine<T>::GetActiveState() {
		return this->_game_states_[this->_active_state_];
	}
	
	template <class T>
	T  Machine<T>::GetActiveStateKey() {
		return this->_active_state_;
	}

	template <class T>
	T  Machine<T>::Update(float elapsed_time) {
		MachineState<T>* active_m_state = this->_game_states_[this->_active_state_];
		if (active_m_state == NULL) {
			/* TODO report an error here */
			return this->_active_state_;
		}
	
		T new_state = this->_game_states_[this->_active_state_]->Update(elapsed_time);
		MachineState<T>* new_m_state = this->_game_states_[new_state];
		if (new_m_state != NULL) {
			active_m_state->UnregisterState(new_m_state);
			this->_active_state_ = new_state;
			new_m_state->RegisterState(active_m_state);
		}
		return this->_active_state_;
	}

}