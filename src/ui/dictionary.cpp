#include "stdafx.h"
#include "dictionary.h"

#include "game.h"
#include "assets/gameasset_manager.h"
#include "events/event_manager.h"
#include "references/refs.h"
#include "tools/sfm.h"
#include "tools/tools.h"

namespace sun_magic {

	Dictionary::Dictionary(float hide_x, float hide_y, float show_x, float show_y, float width, float height, bool is_vertical) : 
		//GameObject(hide_x,hide_y,width,height),
		GameObject(show_x, show_y, width, height),
		hide_pos_(sf::Vector2f(hide_x, hide_y)),
		show_pos_(sf::Vector2f(show_x, show_y)),
		is_vertical_(is_vertical),
		move_animation_(this, .3f)
	{
		font = GameAssetManager::GetInstance()->GetFont(this, refs::fonts::KAORI);
		english_font = GameAssetManager::GetInstance()->GetFont(this, refs::fonts::MSMINCHO);
	}
	Dictionary::~Dictionary() {
		GameAssetManager::GetInstance()->ReturnFonts(this);
	}

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
		//move_animation_.Update(elapsed_time);
	}
	void Dictionary::Draw(sf::RenderTarget *target) {
		sf::Vector2f size = GetSize();

		sf::RectangleShape rect(size);
		rect.setFillColor(sf::Color::White);
		rect.setOutlineColor(sf::Color::Black);
		rect.setOutlineThickness(1);
		target->draw(rect);

		const float padding = 20;
		float x = padding;
		float y = padding;

		sf::Text text;
		text.setFont(*english_font);
		text.setColor(sf::Color::Blue);
		text.setCharacterSize(25);
		text.setPosition(x, y);

		if (is_vertical_) {
			text.setString(sf::String("D\nI\nC\nT\nI\nO\nN\nA\nR\nY"));
			x += text.getGlobalBounds().width + padding;
		} else {
			text.setString(sf::String("DICTIONARY"));
			y += text.getGlobalBounds().height + padding;
		}
		target->draw(text);

		for (std::map<sf::String, DictionaryEntry>::iterator iter = entries_.begin(); iter != entries_.end(); iter++) {
			sf::Sprite sprite = iter->second.sprite;
			sprite.setPosition(x, y);
			target->draw(sprite);
			sf::FloatRect sprite_bounds = sprite.getGlobalBounds();

			sf::Text text(iter->first);
			text.setFont(*font);
			text.setColor(iter->second.outline);
			text.setCharacterSize(50);

			if (is_vertical_) {
				text.setPosition(x + padding + sprite_bounds.width, y + sprite_bounds.height/2 - text.getLocalBounds().height/2);
				y += std::max(sprite_bounds.height, text.getGlobalBounds().height) + padding;
			} else {
				text.setPosition(x + sprite_bounds.width/2 - text.getGlobalBounds().width/2, y + padding + sprite_bounds.height);
				x += std::max(sprite_bounds.width, text.getGlobalBounds().width) + padding;
			}

			target->draw(text);
		}
	}

	void Dictionary::ProcessEvent(Event event) {
		switch (event.type) {
		case Event::E_MOUSE_ENTERED:
			move_animation_.SetEnd(show_pos_);
			break;
		case Event::E_MOUSE_EXITED:
			move_animation_.SetEnd(hide_pos_);
			break;
		}
	}
}
