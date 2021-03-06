#include "stdafx.h"
#include "key_object.h"

#include "assets/gameasset_manager.h"
#include "game.h"
#include "events/event_manager.h"

namespace sun_magic {

	const sf::Color KeyObject::OUTLINE_COLORS[16] = {
		sf::Color::Black
	};

	KeyObject::KeyObject(float x, float y, sf::Sprite sprite, sf::Color outline, sf::String word, key_callback_t callback, bool active, bool visible) :
		GameObject(x, y, (float)sprite.getTexture()->getSize().x, (float)sprite.getTexture()->getSize().y),
		outline_(outline),
		word_(word),
		active_(active),
		focused_(false),
		visible_(visible),
		callback_(callback),
		state_(DEFAULT)
	{
		this->rect_.width = sprite.getTextureRect().width;
		this->rect_.height = sprite.getTextureRect().height;
		SetSprite(sprite);
	}

	KeyObject::KeyObject(float x, float y, sf::String texture_name, sf::Color outline, sf::String word, key_callback_t callback, bool active, bool visible) :
		GameObject(x, y, 0, 0),
		outline_(outline),
		word_(word),
		active_(active),
		focused_(false),
		visible_(visible),
		callback_(callback),
		state_(DEFAULT)
	{
		GameAssetManager* manager = GameAssetManager::GetInstance();
		sf::Texture* texture = manager->GetTexture(texture_name);
		this->rect_.width = texture->getSize().x;
		this->rect_.height = texture->getSize().y;
		SetTexture(*texture);
	}

	KeyObject::~KeyObject()
	{
	}

	void KeyObject::SetSprite(sf::Sprite sprite) {
		sprite_ = sprite;

		sf::RenderTexture temp;
		temp.create(sprite.getTextureRect().width, sprite.getTextureRect().height);
		temp.draw(sprite);

		sf::Image outline_image = temp.getTexture().copyToImage();
		outline_image.flipVertically();
		sf::Vector2u size = outline_image.getSize();

		sf::Image new_image;
		new_image.create(size.x, size.y, sf::Color::Transparent);

		int i, j, min_i, max_i, min_j, max_j, local_i, local_j;
		int radius = 7;
		for (i = 0; i < size.x; ++i) {
			for (j = 0; j < size.y; ++j) {

				min_i = std::max(0, i - radius);
				max_i = std::min(size.x, (unsigned) i + radius);
				min_j = std::max(0, j - radius);
				max_j = std::min(size.y, (unsigned) j + radius);

				float strength = 0;
				for (local_i = min_i; local_i < max_i; ++local_i) {
					for (local_j = min_j; local_j < max_j; ++local_j) {
						if (outline_image.getPixel(local_i, local_j).a > 50)  {
							float distSquared = std::pow(i - local_i, 2.f) + std::pow(j - local_j, 2.f);
							strength += 1.f / std::pow(distSquared, 1.5f);
						}
					}
				}
				
				if (strength > 0) {
					outline_.a = std::min(255.f, 255.f * 1.5f * strength);
					new_image.setPixel(i, j, outline_);
				}
			}
		}

		texture_outline_.loadFromImage(new_image);
		sprite_outline_.setTexture(texture_outline_, true);
	}
	sf::Sprite KeyObject::GetSprite() {
		return sprite_;
	}

	void KeyObject::SetTexture(const sf::Texture& texture) {
		SetSprite(sf::Sprite(texture));
	}
	const sf::Texture& KeyObject::GetTexture() {
		return *sprite_.getTexture();
	}

	void KeyObject::SetOutlineColor(sf::Color outline) {
		if (outline_ == outline)
			return;
		outline_ = outline;
		SetTexture(*sprite_.getTexture());
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

	void KeyObject::SetFocused(bool focused) {
		focused_ = focused;
	}
	bool KeyObject::IsFocused() {
		return focused_;
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
		event_manager->RegisterListener(Event::E_MOUSE_RELEASED, this, NULL);

		event_manager->RegisterListener(Event::E_HIRAGANA_DRAWN, this);
		event_manager->RegisterListener(Event::E_LOAD_STATE, this);
	}
	void KeyObject::Unregister() {
		EventManager *event_manager = Game::GetInstance()->GetEventManager();
		event_manager->UnregisterListener(Event::E_MOUSE_ENTERED, this, this);
		event_manager->UnregisterListener(Event::E_MOUSE_EXITED, this, this);

		event_manager->UnregisterListener(Event::E_MOUSE_PRESSED, this, this);
		event_manager->UnregisterListener(Event::E_MOUSE_RELEASED, this, NULL);

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

		if ((state_ != DEFAULT && active_) || focused_) {
			target->draw(sprite_outline_);
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
			state_ = ACTIVE;
			break;
		case Event::E_MOUSE_RELEASED:
			if (state_ == ACTIVE && event.focus == this) {
				event.focus = this;
				event.type = Event::E_GAME_EVENT;
				event.gameEvent = GameEvent::KEY_CLICK;
				Game::GetInstance()->GetEventManager()->AddEvent(event);
			}
			state_ == OUTLINED;
			break;
		case Event::E_HIRAGANA_DRAWN:
		case Event::E_LOAD_STATE:
			EventCallback(event);
			break;
		}
	}
}