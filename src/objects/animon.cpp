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
		outline_(outline),
		texture_outline_(),
		word_(word),
		active_(active),
		focused_(false),
		visible_(visible),
		state_(DEFAULT),
		animon_state_(AnimonState::UNINITIALIZED),
		meh_texture_(NULL),
		angry_texture_(NULL),
		happy_texture_(NULL)
	{
		GameAssetManager* manager = GameAssetManager::GetInstance();
		sf::Texture *texture = manager->GetTexture(this, refs::textures::objects::SPRITES_OUTLINE);
		sprite_outline_ = manager->GetHiraganaSprite(word_, texture);
		sprite_outline_.setColor(outline);

		// Get our animon's sound
		cry_.setBuffer(*manager->GetSoundBuffer(this, GameAssetManager::hiragana_sound_refs[manager->GetHiraganaIndex(word_)]));
	}

	Animon::~Animon() {
		cry_.stop();
		GameAssetManager::GetInstance()->ReturnTextures(this);
		GameAssetManager::GetInstance()->ReturnSoundBuffers(this);
	}

	void Animon::SetSprite(sf::Sprite sprite) {
		sprite_ = sprite;
		SetSize(sf::Vector2f((float)sprite.getLocalBounds().width, (float)sprite.getLocalBounds().height));
	}
	sf::Sprite Animon::GetSprite() {
		return sprite_;
	}

	void Animon::SetOutlineColor(sf::Color outline) {
		if (outline_ == outline)
			return;
		outline_ = outline;
		sprite_outline_.setColor(outline);
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

	sf::Sound Animon::GetCry() {
		return cry_;
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
			animon_state_ = state;

			sf::Texture* texture = NULL;
			switch (animon_state_) {
			case MEH:
				if (meh_texture_ == NULL)
					meh_texture_ = manager->GetTexture(this, refs::textures::objects::SPRITES_MEH);
				texture = meh_texture_;
				break;
			case HAPPY:
				if (happy_texture_ == NULL)
					happy_texture_ = manager->GetTexture(this, refs::textures::objects::SPRITES_HAPPY);
				texture = happy_texture_;
				break;
			case ANGRY:
				if (angry_texture_ == NULL)
					angry_texture_ = manager->GetTexture(this, refs::textures::objects::SPRITES_ANGRY);
				texture = angry_texture_;
				break;
			}
			SetSprite(manager->GetHiraganaSprite(word_, texture));
		}
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