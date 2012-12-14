#include <stdafx.h>

#include "assets/gameasset_manager.h";
#include "events/event_manager.h";
#include "references/refs.h";
#include "states/credits.h";

namespace sun_magic {

	const int NUM_LABELS = 6;
	sf::String CreditsState::labels_[] = {
		"Amos Yuen: Programmer, Designer",
		"Kellie Medlin: Programmer, Designer, Artist",
		"Dan Vu: Designer",
		"Jessica Cui: Artist",
		"Xavier Artache: Musician",
		"Special thanks to everyone involved with the Animon team!"
	};

	CreditsState::CreditsState() {
		sf::RenderWindow* window = Game::GetInstance()->GetWindow();
		sf::Vector2u size = window->getSize();

		float padding = 10;
		float temp_y = 0;
		
		// init ui group
		UiElement::InitLabel(&credits_group_);
		credits_group_.SetPosition(sf::Vector2f(size.x * .25f, 0.f));
		credits_group_.SetSize(sf::Vector2f(size.x * .6f, (float)size.y));
		credits_group_.GetStyle()->SetNormalColor(sf::Color(100, 100, 100));
		float label_width = credits_group_.GetSize().x;

		UiElement* label = UiElement::InitLabel(new UiElement(0, temp_y, label_width, 75, "Credits"));
		label->GetStyle()->SetNormalColor(sf::Color::Cyan);
		credits_group_.UiAdd(label);
		temp_y += padding + label->GetSize().y;

		for (int i = 0; i < NUM_LABELS; ++i) {
			UiElement* label = new UiElement(label_width * .25f, temp_y, label_width * .5, 50, labels_[i]);
			label->GetStyle()->Clear()->SetTextSize(18)->SetTextColor(sf::Color::White);
			credits_group_.UiAdd(label);
			temp_y += padding + label->GetSize().y;
		}

		UiElement::InitButton(&menu_button_);
		menu_button_.SetSize(sf::Vector2f(label_width * .5, 50));
		menu_button_.SetString("Back to Menu");
		menu_button_.SetPosition(sf::Vector2f(label_width * .25f, temp_y));
		credits_group_.UiAdd(&menu_button_);
	}

	CreditsState::~CreditsState() {
		for (int i = 0; i < credits_group_.UiSize(); ++i)
			delete credits_group_[i];
		credits_group_.UiClear();
	}

	void CreditsState::RegisterState(MachineState<GameState>* previous_state) {
		game_state_ = GameState::CREDITS;

		background_.setTexture(*GameAssetManager::GetInstance()->GetTexture(this, refs::textures::backgrounds::POSTER_AWAY));

		sf::Font* font = GameAssetManager::GetInstance()->GetFont(this, refs::fonts::MSMINCHO);
		for (int i = 0; i < NUM_LABELS; ++i)
			((UiElement*)credits_group_[i])->GetStyle()->SetTextFont(*font);

		Game::GetInstance()->GetEventManager()->AddGameObject(&credits_group_);
		Game::GetInstance()->GetEventManager()->RegisterListener(Event::E_CLICKED, this, NULL);
	}

	void CreditsState::UnregisterState(MachineState<GameState>* next_state) {
		GameAssetManager::GetInstance()->ReturnTextures(this);
		Game::GetInstance()->GetEventManager()->RemoveGameObject(&credits_group_);
		Game::GetInstance()->GetEventManager()->UnregisterListener(Event::E_CLICKED, this, NULL);
	}

	GameState CreditsState::Update(float elapsed_time) {
		return game_state_;
	}

	void CreditsState::PreDraw(sf::RenderTarget *target) {
		target->draw(background_);
	}

	void CreditsState::PostDraw(sf::RenderTarget *target) {
	}

	void CreditsState::ProcessEvent(sun_magic::Event event) {
		if (event.type == Event::E_CLICKED && event.focus == &menu_button_) {
			game_state_ = GameState::MAIN_MENU;
		}
	}

}