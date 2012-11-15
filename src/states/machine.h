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

		bool Init(T initial_state, MachineState<T>* initial_machine_state);
		MachineState<T>* AddState(T state, MachineState<T>* machine_state);
		MachineState<T>* RemoveState(T state);
		MachineState<T>* GetActiveState();
		T GetActiveStateKey();
		T Update(float elapsed_time);

	protected:
		std::hash_map<T, MachineState<T>*> machine_states_;
		T current_state_;
	};


	template <class T>
	bool Machine<T>::Init(T initial_state, MachineState<T>* initial_machine_state) {
		this->current_state_ = initial_state;
		this->AddState(initial_state, initial_machine_state);
		initial_machine_state->RegisterState(NULL);
		return true;
	}

	template <class T>
	MachineState<T>* Machine<T>::AddState(T state, MachineState<T>* machine_state) {
		if (machine_state == NULL) {
			return NULL;
		}

		MachineState<T>* old_state = this->machine_states_[state];
		this->machine_states_[state] = machine_state;
		return old_state;
	}

	template <class T>
	MachineState<T>* Machine<T>::RemoveState(T state) {
		MachineState<T>* removed_state = this->machine_states_[state];
		this->machine_states_[state] = NULL;
		return removed_state;
	}
	
	template <class T>
	MachineState<T>*  Machine<T>::GetActiveState() {
		return this->machine_states_[this->current_state_];
	}
	
	template <class T>
	T  Machine<T>::GetActiveStateKey() {
		return this->current_state_;
	}

	template <class T>
	T  Machine<T>::Update(float elapsed_time) {
		MachineState<T>* active_m_state = this->machine_states_[this->current_state_];
		if (active_m_state == NULL) {
			throw "NULL machine state";
		}
	
		T new_state = this->machine_states_[this->current_state_]->Update(elapsed_time);
		if (new_state != current_state_) {
			MachineState<T>* new_m_state = this->machine_states_[new_state];
			if (new_m_state == NULL) {
				throw "NULL machine state";
			}

			active_m_state->UnregisterState(new_m_state);
			this->current_state_ = new_state;
			new_m_state->RegisterState(active_m_state);
		}
		return this->current_state_;
	}

}