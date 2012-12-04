#pragma once

#include "stdafx.h"
#include "events/event.h"
#include "states/game_state.h"
#include "states/machine_state.h"
#include "ui/character_tilelist.h"

namespace sun_magic {

	class Animon;
	class Foodbowl;

	class Feeding : public MachineState<GameState>, public EventListener {
	public:
		Feeding();
		~Feeding();

		void SetHiraganas(std::vector<sf::String> hiraganas);

		void RegisterState(MachineState<GameState>* previous_state);
		void UnregisterState(MachineState<GameState>* next_state);

		GameState Update(float elapsed_time);
		void PreDraw(sf::RenderTarget *target);
		void PostDraw(sf::RenderTarget *target);

		void ProcessEvent(Event event);

	private:
		sf::Sprite background_;
		std::vector<sf::String> hiraganas_;
		std::vector<Animon*> animons_;
		std::vector<Foodbowl*> foodbowls_;
		GameState game_state_;
	};

}