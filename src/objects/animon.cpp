#include "stdafx.h"
#include "animon.h"

#include "game.h"
#include "assets/gameasset_manager.h"
#include "events/event_manager.h"

namespace sun_magic {

	const sf::Color Animon::OUTLINE_COLORS[16] = {
		sf::Color::Black
	};

	Animon::Animon(float x, float y, sf::Texture& texture, sf::Color outline, sf::String word, bool active, bool visible) :
		GameObject(x, y, (float)texture.getSize().x, (float)texture.getSize().y),
		outline_(outline),
		word_(word),
		active_(active),
		focused_(false),
		visible_(visible),
		state_(DEFAULT)
	{
		SetTexture(texture);
	}

	Animon::Animon(float x, float y, sf::String texture_name, sf::Color outline, sf::String word, bool active, bool visible) :
		GameObject(x, y, 0, 0),
		outline_(outline),
		word_(word),
		active_(active),
		focused_(false),
		visible_(visible),
		state_(DEFAULT)
	{
		GameAssetManager* manager = GameAssetManager::GetInstance();
		sf::Texture* texture = manager->GetTexture(texture_name);

		this->rect_.width = (float)texture->getSize().x;
		this->rect_.height = (float)texture->getSize().y;

		SetTexture(*texture);
	}

	Animon::~Animon()
	{
	}

	void Animon::SetTexture(const sf::Texture& texture) {
		sprite_.setTexture(texture);

		sf::Image outline_image = texture.copyToImage();
		sf::Vector2u size = outline_image.getSize();

		sf::Image new_image;
		new_image.create(size.x, size.y, sf::Color::Transparent);

		int i, j, min_i, max_i, min_j, max_j, local_i, local_j;
		int radius = 7;
		for (i = 0; i < (int)size.x; ++i) {
			for (j = 0; j < (int)size.y; ++j) {

				min_i = std::max(0, i - radius);
				max_i = std::min(size.x, (unsigned) i + radius);
				min_j = std::max(0, j - radius);
				max_j = std::min(size.y, (unsigned) j + radius);

				float strength = 0;
				for (local_i = min_i; local_i < max_i; ++local_i) {
					for (local_j = min_j; local_j < max_j; ++local_j) {
						if (outline_image.getPixel(local_i, local_j).a > 50)  {
							float distSquared = std::pow((float)(i - local_i), 2.f) + std::pow((float)(j - local_j), 2.f);
							strength += 1.f / std::pow(distSquared, 1.5f);
						}
					}
				}
				
				if (strength > 0) {
					outline_.a = sf::Uint8(std::min(255.f, 255.f * 1.5f * strength));
					new_image.setPixel(i, j, outline_);
				}
			}
		}

		texture_outline_.loadFromImage(new_image);
		sprite_outline_.setTexture(texture_outline_, true);
	}
	const sf::Texture& Animon::GetTexture() {
		return *sprite_.getTexture();
	}

	void Animon::SetOutlineColor(sf::Color outline) {
		if (outline_ == outline)
			return;
		outline_ = outline;
		SetTexture(*sprite_.getTexture());
	}
	sf::Color Animon::GetOutlineColor() {
		return outline_;
	}

	void Animon::SetWord(sf::String word) {
		word_ = word;
	}
	sf::String Animon::GetWord() {
		return word_;
	}

	void Animon::SetFocused(bool focused) {
		focused_ = focused;
	}
	bool Animon::IsFocused() {
		return focused_;
	}

	void Animon::SetActive(bool active) {
		active_ = active;
	}
	bool Animon::IsActive() {
		return active_;
	}

	void Animon::SetVisible(bool visible) {
		visible_ = visible;
	}
	bool Animon::IsVisible() {
		return visible_;
	}

	void Animon::Register() {
		EventManager *event_manager = Game::GetInstance()->GetEventManager();
		event_manager->RegisterListener(Event::E_MOUSE_ENTERED, this, this);
		event_manager->RegisterListener(Event::E_MOUSE_EXITED, this, this);

		event_manager->RegisterListener(Event::E_MOUSE_PRESSED, this, this);
		event_manager->RegisterListener(Event::E_MOUSE_RELEASED, this, NULL);

		event_manager->RegisterListener(Event::E_HIRAGANA_DRAWN, this);
		event_manager->RegisterListener(Event::E_LOAD_STATE, this);
	}
	void Animon::Unregister() {
		EventManager *event_manager = Game::GetInstance()->GetEventManager();
		event_manager->UnregisterListener(Event::E_MOUSE_ENTERED, this, this);
		event_manager->UnregisterListener(Event::E_MOUSE_EXITED, this, this);

		event_manager->UnregisterListener(Event::E_MOUSE_PRESSED, this, this);
		event_manager->UnregisterListener(Event::E_MOUSE_RELEASED, this, NULL);

		event_manager->UnregisterListener(Event::E_HIRAGANA_DRAWN, this);
		event_manager->UnregisterListener(Event::E_LOAD_STATE, this);
	}

	void Animon::Update(float elapsed_time) {
	}
	void Animon::Draw(sf::RenderTarget* target) {
		if (!visible_)
			return;

		sf::Vector2f size(rect_.width, rect_.height);
		sf::Vector2f texture_size = sf::Vector2f(sprite_.getTexture()->getSize());
		sprite_.setPosition((size - texture_size) * 0.5f);
		sprite_.setOrigin(sf::Vector2f());
		if ((state_ != DEFAULT && active_) || focused_) {
			target->draw(sprite_outline_);
		}
		target->draw(sprite_);
	}

	void Animon::ProcessEvent(Event event) {
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
			if (state_ == ACTIVE) {
				event.source = this;
				event.type = Event::E_GAME_EVENT;
				event.gameEvent = GameEvent::KEY_CLICK;
				event.message = word_;
				Game::GetInstance()->GetEventManager()->AddEvent(event);
			}
			state_ = OUTLINED;
			break;
		}
	}
}