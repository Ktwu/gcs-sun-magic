#include "stdafx.h"
#include "animon.h"

#include "game.h"
#include "assets/gameasset_manager.h"
#include "events/event_manager.h"
#include "references/refs.h"

namespace sun_magic {

	const int RADIUS = 7;

	Animon::Animon(float x, float y, sf::Color outline, sf::String word, bool active, bool visible) :
		GameObject(x, y, 0, 0),
		sprite_(),
		sprite_outline_(),
		outline_(outline),
		texture_outline_(),
		word_(word),
		active_(active),
		focused_(false),
		visible_(visible),
		state_(DEFAULT),
		animon_state_(AnimonState::UNINITIALIZED),
		thread_(NULL)
	{
		sprite_.setPosition(0, 0);
	}

	Animon::~Animon()
	{
		WaitForLoadState();
	}

	void Animon::SetSprite(sf::Sprite sprite) {
		sprite_ = sprite;
		SetSize(sf::Vector2f((float)sprite.getLocalBounds().width, (float)sprite.getLocalBounds().height));
		
		sf::IntRect rect = sprite.getTextureRect();
		sf::RenderTexture temp;
		temp.create(rect.width + 2 * RADIUS, rect.height + 2 * RADIUS);
		temp.draw(sprite);

		sf::Image outline_image = temp.getTexture().copyToImage();
		outline_image.flipVertically();
		sf::Vector2u outline_size = outline_image.getSize();

		sf::Image new_image;
		new_image.create(outline_size.x, outline_size.y, sf::Color::Transparent);

		int outline_i, outline_j, i, j, max_i, max_j, min_i, min_j, local_i, local_j;
		for (outline_i = 0; outline_i < (int)outline_size.x; ++outline_i) {
			for (outline_j = 0; outline_j < (int)outline_size.y; ++outline_j) {
				i = outline_i - RADIUS;
				j = outline_j - RADIUS;
				min_i = std::max(0, i - RADIUS);
				max_i = std::min(rect.width, i + RADIUS);
				min_j = std::max(0, j - RADIUS);
				max_j = std::min(rect.height, j + RADIUS);

				float strength = 0;
				for (local_i = min_i; local_i < max_i; ++local_i) {
					for (local_j = min_j; local_j < max_j; ++local_j) {
						if (outline_image.getPixel(local_i, local_j).a > 0)  {
							float distSquared = std::pow((float)(i - local_i), 2.f) + std::pow((float)(j - local_j), 2.f);
							strength += 1.f / std::pow(distSquared, 1.5f);
						}
					}
				}
				
				if (strength > 0) {
					outline_.a = sf::Uint8(std::min(255.f, 255.f * 1.5f * strength));
					new_image.setPixel(outline_i, outline_j, outline_);
				}
			}
		}

		texture_outline_.loadFromImage(new_image);
		sprite_outline_.setTexture(texture_outline_, true);
	}
	sf::Sprite Animon::GetSprite() {
		return sprite_;
	}

	void Animon::SetTexture(const sf::Texture& texture) {
		SetSprite(sf::Sprite(texture));
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

	void Animon::LoadState(AnimonState state) {
		GameAssetManager* manager = GameAssetManager::GetInstance();

		if (state != animon_state_) {
			switch (animon_state_) {
			case MEH:
				manager->ReturnTexture(refs::textures::objects::SPRITES_MEH);
				break;
			case HAPPY:
				manager->ReturnTexture(refs::textures::objects::SPRITES_HAPPY);
				break;
			case ANGRY:
				manager->ReturnTexture(refs::textures::objects::SPRITES_ANGRY);
				break;
			}

			animon_state_ = state;

			WaitForLoadState();
			thread_ = new sf::Thread(Animon::ThreadLoad, this);
			thread_->launch();
		}
	}
	void Animon::WaitForLoadState() {
		if (thread_ != NULL) {
			thread_->wait();
			delete thread_;
		}
		thread_ = NULL;
	}

	void Animon::Register() {
		EventManager *event_manager = Game::GetInstance()->GetEventManager();
		event_manager->RegisterListener(Event::E_MOUSE_ENTERED, this, this);
		event_manager->RegisterListener(Event::E_MOUSE_EXITED, this, this);
		event_manager->RegisterListener(Event::E_MOUSE_PRESSED, this, this);
		event_manager->RegisterListener(Event::E_MOUSE_RELEASED, this, this);
	}
	void Animon::Unregister() {
		EventManager *event_manager = Game::GetInstance()->GetEventManager();
		event_manager->UnregisterListener(Event::E_MOUSE_ENTERED, this, this);
		event_manager->UnregisterListener(Event::E_MOUSE_EXITED, this, this);
		event_manager->UnregisterListener(Event::E_MOUSE_PRESSED, this, this);
		event_manager->UnregisterListener(Event::E_MOUSE_RELEASED, this, this);
	}

	void Animon::Update(float elapsed_time) {
	}
	void Animon::Draw(sf::RenderTarget* target) {
		if (!visible_)
			return;

		if ((state_ != DEFAULT && active_) || focused_) {
			sprite_outline_.setPosition(sprite_.getPosition() - sf::Vector2f(RADIUS,RADIUS));
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

	void Animon::ThreadLoad(Animon* animon) {
		GameAssetManager* manager = GameAssetManager::GetInstance();
		sf::Texture* texture;
		switch (animon->animon_state_) {
		case MEH:
			texture = manager->GetTexture(refs::textures::objects::SPRITES_MEH);
			break;
		case HAPPY:
			texture = manager->GetTexture(refs::textures::objects::SPRITES_HAPPY);
			break;
		case ANGRY:
			texture = manager->GetTexture(refs::textures::objects::SPRITES_ANGRY);
			break;
		}
		animon->SetSprite(manager->GetHiraganaSprite(animon->word_, texture));
	}
}