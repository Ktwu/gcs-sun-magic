#include "stdafx.h"
#include "key_object.h"

#include "assets/gameasset_manager.h"
#include "game.h"
#include "events/event_manager.h"

namespace sun_magic {

	const sf::Color KeyObject::OUTLINE_COLORS[16] = {
		sf::Color::Black
	};

	KeyObject::KeyObject(float x, float y, sf::Texture& texture, sf::Color outline, sf::String word, key_callback_t callback, bool active, bool visible) :
		GameObject(x, y, (float)texture.getSize().x, (float)texture.getSize().y),
		sprite_(texture),
		outline_(outline),
		word_(word),
		active_(active),
		visible_(visible),
		callback_(callback)
	{
	}

	KeyObject::KeyObject(float x, float y, sf::String texture_name, sf::Color outline, sf::String word, key_callback_t callback, bool active, bool visible) :
		GameObject(x, y, 0, 0),
		outline_(outline),
		word_(word),
		active_(active),
		visible_(visible),
		callback_(callback)
	{
		GameAssetManager* manager = GameAssetManager::GetInstance();
		sf::Texture* texture = manager->GetTexture(texture_name);

		this->rect_.width = texture->getSize().x;
		this->rect_.height = texture->getSize().y;
		this->sprite_.setTexture(*texture, true);
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

	void KeyObject::SetVisible(bool visible) {
		visible_ = visible;
	}
	bool KeyObject::IsVisible() {
		return visible_;
	}

	void KeyObject::EventCallback(Event event) {
		if (callback_ != NULL)
			callback_(this, event);
	}
	void KeyObject::SetEventCallback(key_callback_t callback) {
		callback_ = callback;
	}

	void KeyObject::Register() {
		EventManager *event_manager = Game::GetInstance()->GetEventManager();
		event_manager->RegisterListener(Event::E_MOUSE_ENTERED, this, this);
		event_manager->RegisterListener(Event::E_MOUSE_EXITED, this, this);
		event_manager->RegisterListener(Event::E_MOUSE_PRESSED, this, this);
		event_manager->RegisterListener(Event::E_MOUSE_RELEASED, this, this);

		event_manager->RegisterListener(Event::E_HIRAGANA_DRAWN, this);
		event_manager->RegisterListener(Event::E_LOAD_STATE, this);
	}
	void KeyObject::Unregister() {
		EventManager *event_manager = Game::GetInstance()->GetEventManager();
		event_manager->UnregisterListener(Event::E_MOUSE_ENTERED, this, this);
		event_manager->UnregisterListener(Event::E_MOUSE_EXITED, this, this);
		event_manager->UnregisterListener(Event::E_MOUSE_PRESSED, this, this);
		event_manager->UnregisterListener(Event::E_MOUSE_RELEASED, this, this);

		event_manager->UnregisterListener(Event::E_HIRAGANA_DRAWN, this);
		event_manager->UnregisterListener(Event::E_LOAD_STATE, this);
	}

	void KeyObject::Update(float elapsed_time) {
	}
	void KeyObject::Draw(sf::RenderTarget* target) {
		if (!visible_)
			return;

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
		case Event::E_HIRAGANA_DRAWN:
		case Event::E_LOAD_STATE:
			EventCallback(event);
			break;
		}
	}
}