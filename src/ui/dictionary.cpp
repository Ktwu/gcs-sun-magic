#include "stdafx.h"
#include "dictionary.h"

#include "assets/gameasset_manager.h"
#include "game.h"
#include "events/event_manager.h"
#include "references/refs.h"
#include "tools/sfm.h"

namespace sun_magic {

	Dictionary::Dictionary(int hide_x, int hide_y, int show_x, int show_y, int width, int height) : 
		GameObject(0,0,0,0)
	{
		animate_speed_ = 600;
		sf::Vector2u size = Game::GetInstance()->GetWindow()->getSize();
		hide_pos_ = sf::Vector2f(hide_x, hide_y);
		show_pos_ = sf::Vector2f(show_x, show_y);
		target_pos_ = hide_pos_;
		rect_ = sf::FloatRect(hide_pos_, sf::Vector2f(width, height));
	}
	Dictionary::~Dictionary() {}

	void Dictionary::AddWord(sf::String word, sf::Texture* texture) {
		DictionaryEntry entry;
		entry.texture = texture;
		entry.outline = GetColor();
		entries_[word] = entry;
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
		sf::Vector2f position = GetPosition();
		sf::Vector2f size = GetSize();

		sf::RectangleShape rect(size);
		rect.setFillColor(sf::Color::White);
		rect.setOutlineColor(sf::Color::Black);
		rect.setOutlineThickness(1);
		target->draw(rect);

		const float padding = 10;
		float y = padding;

		sf::Font* msmincho = GameAssetManager::GetInstance()->GetFont(refs::fonts::MSMINCHO);
		sf::Text text(sf::String("DICTIONARY"));
			text.setFont(*msmincho);
			text.setColor(sf::Color::Blue);
			text.setCharacterSize(25);
			text.setPosition(2.f * padding, y);
			target->draw(text);
			y += text.getLocalBounds().height + padding;

		for (std::map<sf::String, DictionaryEntry>::iterator iter = entries_.begin(); iter != entries_.end(); iter++) {
			sf::Sprite sprite;
			sprite.setTexture(*iter->second.texture);
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

	/* Returns the next color with the most H constract in the HSV space */
	sf::Color Dictionary::GetColor() {
		static unsigned int index = 0;
		static unsigned int count = 1;
		static float h = 0.0f;

		float s = 1;
		float v = 1;

		float h2 = h * 6;
		float c = s * v;
		float x = c * (1 - abs(fmod(h2, 2.f) - 1));
		sf::Color color;
		switch ((int)h2) {
		case 0:
			color = sf::Color(c,x,0);
			break;
		case 1:
			color = sf::Color(x,c,0);
			break;
		case 2:
			color = sf::Color(0,c,x);
			break;
		case 3:
			color = sf::Color(0,x,c);
			break;
		case 4:
			color = sf::Color(x,0,c);
			break;
		case 5:
			color = sf::Color(c,0,x);
			break;
		}

		index++;
		if (index == 1) {
			h += 0.5f / count;
		} else {
			h += 1.f / count;
		}
		if (index == count) {
			count *= 2;
			index = 0;
		}
		return color;
	}
}
