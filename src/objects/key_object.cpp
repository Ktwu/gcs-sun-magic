#include "stdafx.h"
#include "key_object.h"

#include "game.h"
#include "events/event_manager.h"

namespace sun_magic {

	KeyObject::KeyObject(float x, float y, sf::Texture& texture, sf::Color outline, sf::String word, bool active) :
		GameObject(x, y, (float)texture.getSize().x, (float)texture.getSize().y),
		sprite_(texture),
		outline_(outline),
		word_(word),
		active_(active)
	{
	}

	KeyObject::~KeyObject()
	{
	}

	
	void KeyObject::SetTexture(const sf::Texture& texture) {
		sprite_.setTexture(texture);
	}
	const sf::Texture& KeyObject::GetTexture() {
		return *sprite_.getTexture();
	}

	void KeyObject::SetOutlineColor(sf::Color outline) {
		outline_ = outline;
	}
	sf::Color KeyObject::GetOutlineColor() {
		return outline_;
	}

	void KeyObject::SetWord(sf::String word) {
		word_ = word;
	}
	sf::String KeyObject::GetWord() {
		return word_;
	}

	void KeyObject::SetActive(bool active) {
		active_ = active;
	}
	bool KeyObject::IsActive() {
		return active_;
	}

	void KeyObject::Register() {
		EventManager *event_manager = Game::GetInstance()->GetEventManager();
		event_manager->RegisterListener(Event::E_MOUSE_ENTERED, this, this);
		event_manager->RegisterListener(Event::E_MOUSE_EXITED, this, this);
		event_manager->RegisterListener(Event::E_MOUSE_PRESSED, this, this);
		event_manager->RegisterListener(Event::E_MOUSE_RELEASED, this, this);
	}
	void KeyObject::Unregister() {
		EventManager *event_manager = Game::GetInstance()->GetEventManager();
		event_manager->UnregisterListener(Event::E_MOUSE_ENTERED, this, this);
		event_manager->UnregisterListener(Event::E_MOUSE_EXITED, this, this);
		event_manager->UnregisterListener(Event::E_MOUSE_PRESSED, this, this);
		event_manager->UnregisterListener(Event::E_MOUSE_RELEASED, this, this);
	}

	void KeyObject::Update(float elapsed_time) {
	}
	void KeyObject::Draw(sf::RenderTarget* target) {
		sf::Vector2f size(rect_.width, rect_.height);
		sf::Vector2f texture_size = sf::Vector2f(sprite_.getTexture()->getSize());
		sprite_.setPosition((size - texture_size) * 0.5f);
		sprite_.setOrigin(sf::Vector2f());
		if (state_ != DEFAULT && active_) {
			// Draw outline
			sprite_.setColor(outline_);
			sprite_.setScale((texture_size.x + OUTLINE_WIDTH) / texture_size.x, (texture_size.y + OUTLINE_WIDTH) / texture_size.y);
			target->draw(sprite_);
			sprite_.setScale(1,1);
			sprite_.setColor(sf::Color::White);
		}
		target->draw(sprite_);
	}

	void KeyObject::ProcessEvent(Event event) {
		switch (event.type) {
		case Event::E_MOUSE_ENTERED:
			state_ = OUTLINED;
			break;
		case Event::E_MOUSE_EXITED:
			state_ = DEFAULT;
			break;
		case Event::E_MOUSE_PRESSED:
			if (active_) {
				state_ = ACTIVE;
			}
			break;
		}
	}
}