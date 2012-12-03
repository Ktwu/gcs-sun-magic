#pragma once

#include "stdafx.h"
#include "animon.h"

#include "assets/gameasset_manager.h"
#include "events/event_manager.h"
#include "game.h"
#include "objects/key_object.h"
#include "tools/tools.h"

namespace sun_magic {

#define POINTS 100
#define GROWTH 10

	Animon::Animon(float x, float y, sf::String texture_name, sf::Color outline, sf::String word) :
		KeyObject(x, y, texture_name, outline, word),
		happiness_(0.0f),
		health_(100.0f),
		level_(1),
		exist_time_(5.0f),
		done_(false)
	{
	}

	Animon::Animon(float x, float y, sf::Sprite sprite, sf::Color outline, sf::String word) :
		KeyObject(x, y, sprite, outline, word),
		happiness_(0.0f),
		health_(100.0f),
		level_(1),
		exist_time_(5.0f),
		done_(false)
	{
	}

	Animon::~Animon()
	{
	}

	void Animon::Update(float elapsed_time) {
		/*if (this->timer_.getElapsedTime().asSeconds() > exist_time_ && !done_) {
			// Remove the animon, update player score, etc etc
			this->SetOutlineColor(sf::Color::Green);
			done_ = true;
			return;
		}*/

		// Every turn, use some algorithm to make us more sick.
		// Use a variable threshold for death
		/*this->health_ += 0.01;
		if (this->health_ < 0.0f && !done_) {
			// DEATH :(
			this->SetOutlineColor(sf::Color::Black);
			done_ = true;
			return;
		}*/

		// Jitter our movement around on the board.  Get some random movement n'at
		/*this->rect_.top += std::rand() % 3 - 1;
		this->rect_.left += std::rand() % 3 - 1;
		sf::RenderWindow* window = Game::GetInstance()->GetWindow();
		this->rect_.top = tools::LimitBetween(100, this->rect_.top, window->getSize().y);
		this->rect_.left = tools::LimitBetween(100, this->rect_.left, window->getSize().x - 100);*/
	}

	bool Animon::IsWordWritten(Event event) {
		/* Depending on our level, we check to see if the player wrote so many words in */
		if (event.message.getSize() < word_.getSize() && event.message.getSize() < level_)
			return false;

		int min_len = std::min(word_.getSize(), (size_t) level_);
		for (int i = 0; i < min_len; ++i)
			if (word_[i] != event.message[i])
				return false;
		return true;
	}

	void Animon::Grow() {
		std::cout << "Grow!\n";
		float health_points = POINTS;
		float happy_points = 0;

		if (health_ + health_points > POINTS * level_) {
			health_points = POINTS * level_ - health_;
			happy_points = POINTS - health_points;
		}

		health_ += health_points;
		happiness_ += happy_points;

		/* Try to level up! */
		if (happiness_ > POINTS * level_) {
			++level_;
			health_ = POINTS * level_;
			happiness_ = 0;
			Event level_up;
			level_up.type = Event::E_GAME_EVENT;
			level_up.gameEvent = GameEvent::ANIMON_LEVEL_UP;
			level_up.focus = this;

			Game::GetInstance()->GetEventManager()->AddEvent(level_up);

			if (level_ == 2)
				SetOutlineColor(sf::Color::Blue);
			else if (level_ == 3)
				SetOutlineColor(sf::Color::Red);
		}
	}

}