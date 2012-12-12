#pragma once

#include "stdafx.h"
#include "events/event.h"
#include "states/game_state.h"
#include "states/machine_state.h"
#include "ui/character_tile.h"
#include "ui/ui_element.h"
#include "ui/ui_group.h"

namespace sun_magic {

	class NewLevelState : public MachineState<GameState>, public EventListener {
	public:
		NewLevelState();
		~NewLevelState();

		sf::String GetCurrentLevelHiragana();

		void RegisterState(MachineState<GameState>* previous_state);
		void UnregisterState(MachineState<GameState>* next_state);

		GameState Update(float elapsed_time);
		void PreDraw(sf::RenderTarget *target);
		void PostDraw(sf::RenderTarget *target);

		void ProcessEvent(Event event);

	private:
		static bool MapSort(std::pair<sf::String, int> a, std::pair<sf::String, int> b);
		sf::String GetNewLevelHiragana();

		static const int NUM_HIRAGANA_LABELS = 5;

		GameState state_;
		sf::Sprite background_;
		sf::String hiraganas_;

		UiElement start_button_;

		UiGroup intro_display_;
		UiElement level_label_;
		CharacterTile tile_;
		UiElement hiragana_labels_[NUM_HIRAGANA_LABELS];

		std::vector<sf::String> levels_;
		std::map<sf::String, int> hiragana_scores_;
		sf::String level_hiragana_;
	};

}