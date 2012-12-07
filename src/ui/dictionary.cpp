#include "stdafx.h"
#include "dictionary.h"

#include "game.h"
#include "assets/gameasset_manager.h"
#include "events/event_manager.h"
#include "references/refs.h"
#include "tools/sfm.h"
#include "tools/tools.h"

namespace sun_magic {

	Dictionary::Dictionary(float hide_x, float hide_y, float show_x, float show_y, float width, float height) : 
		GameObject(hide_x,hide_y,width,height)
	{
		animate_time_ = 0.3f;
		sf::Vector2u size = Game::GetInstance()->GetWindow()->getSize();
		hide_pos_ = sf::Vector2f(hide_x, hide_y);
		show_pos_ = sf::Vector2f(show_x, show_y);
		target_pos_ = hide_pos_;
	}
	Dictionary::~Dictionary() {}

	void Dictionary::AddWord(sf::String word, sf::Sprite sprite) {
		DictionaryEntry entry;
		entry.sprite = sprite;
		entries_[word] = entry;
	}

	void Dictionary::Clear() {
		entries_.clear();
	}

	void Dictionary::Register() {
		EventManager* event_manager = Game::GetInstance()->GetEventManager();
		event_manager->RegisterListener(Event::E_MOUSE_ENTERED, this, this);
		event_manager->RegisterListener(Event::E_MOUSE_EXITED, this, this);
	}
	void Dictionary::Unregister() {
		EventManager* event_manager = Game::GetInstance()->GetEventManager();
		event_manager->UnregisterListener(Event::E_MOUSE_ENTERED, this, this);
		event_manager->UnregisterListener(Event::E_MOUSE_EXITED, this, this);
	}

	void Dictionary::Update(float elapsed_time) {
		float animate_speed_ = sfm::Length(hide_pos_ - show_pos_) / animate_time_;
		sf::Vector2f dist = target_pos_ - GetPosition();
		float length = sfm::Length(dist);
		float max_dist = animate_speed_ * elapsed_time;
		if (length > max_dist) {
			dist *= max_dist / length;
		}
		rect_.left += dist.x;
		rect_.top += dist.y;
	}
	void Dictionary::Draw(sf::RenderTarget *target) {
		sf::Vector2f size = GetSize();

		sf::RectangleShape rect(size);
		rect.setFillColor(sf::Color::White);
		rect.setOutlineColor(sf::Color::Black);
		rect.setOutlineThickness(1);
		target->draw(rect);

		const float padding = 20;
		float y = padding;

		sf::Font* msmincho = GameAssetManager::GetInstance()->GetFont(refs::fonts::MSMINCHO);
		sf::Text text(sf::String("D\nI\nC\nT\nI\nO\nN\nA\nR\nY"));
		text.setFont(*msmincho);
		text.setColor(sf::Color::Blue);
		text.setCharacterSize(25);
		text.setPosition(5, y);
		target->draw(text);

		for (std::map<sf::String, DictionaryEntry>::iterator iter = entries_.begin(); iter != entries_.end(); iter++) {
			sf::Sprite sprite = iter->second.sprite;
			sprite.setPosition(padding, y);
			target->draw(sprite);
			sf::FloatRect sprite_bounds = sprite.getLocalBounds();

			sf::Text text(iter->first);
			text.setFont(*msmincho);
			text.setColor(iter->second.outline);
			text.setCharacterSize(50);
			text.setPosition(2.f * padding + sprite_bounds.width, y + sprite_bounds.height/2 - text.getLocalBounds().height/2);
			target->draw(text);
			y += std::max(sprite_bounds.height, text.getLocalBounds().height) + padding;
		}

		GameAssetManager::GetInstance()->ReturnFont(refs::fonts::MSMINCHO);
	}

	void Dictionary::ProcessEvent(Event event) {
		switch (event.type) {
		case Event::E_MOUSE_ENTERED:
			target_pos_ = show_pos_;
			break;
		case Event::E_MOUSE_EXITED:
			target_pos_ = hide_pos_;
			break;
		}
	}
}
