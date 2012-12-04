#pragma once

#include "stdafx.h"
#include "events/event.h"
#include "states/game_state.h"
#include "states/machine_state.h"
#include "ui/character_tilelist.h"

namespace sun_magic {

	class Animon;
	class ProgressBar;

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
		std::vector<ProgressBar*> progressbars_;
		GameState game_state_;
		float feed_increment_;
		float eat_rate_;
		float happy_threshold_;
		float ok_threshold_;
	};

}