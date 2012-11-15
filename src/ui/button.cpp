#include "stdafx.h"
#include "button.h"

#include "game.h"
#include "events/event_manager.h"

namespace sun_magic {

	Button::Button(float x, float y, float width, float height, sf::String text, sf::Texture *texture) :
		GameObject(x, y, width, height),
		string_(text),
		texture_(texture),
		background_(sf::Color(230,230,230)),
		border_(sf::Color(50,50,50)),
		hoverbackground_(sf::Color(200,200,200)),
		hover_border_(sf::Color(100,100,100)),
		pressedbackground_(sf::Color(150,150,150)),
		pressed_border_(sf::Color(20,20,20)),
		state_(NEUTRAL)
	{
	}

	Button::~Button() {}

	sf::String Button::GetString() {
		return string_;
	}
	void Button::SetString(sf::String string) {
		string_ = string;
	}

	sf::Texture* Button::GetTexture() {
		return texture_;
	}
	void Button::SetTexture(sf::Texture *texture) {
		texture_ = texture;
	}

	void Button::Register() {
		Game::GetInstance()->GetEventManager()->RegisterListener(Event::E_MOUSE_ENTERED, this, this);
		Game::GetInstance()->GetEventManager()->RegisterListener(Event::E_MOUSE_EXITED, this, this);
		Game::GetInstance()->GetEventManager()->RegisterListener(Event::E_MOUSE_PRESSED, this, this);
		Game::GetInstance()->GetEventManager()->RegisterListener(Event::E_MOUSE_RELEASED, this, this);
	}
	void Button::Unregister() {
		Game::GetInstance()->GetEventManager()->UnregisterListener(Event::E_MOUSE_ENTERED, this, this);
		Game::GetInstance()->GetEventManager()->UnregisterListener(Event::E_MOUSE_EXITED, this, this);
		Game::GetInstance()->GetEventManager()->UnregisterListener(Event::E_MOUSE_PRESSED, this, this);
		Game::GetInstance()->GetEventManager()->UnregisterListener(Event::E_MOUSE_RELEASED, this, this);
	}

	void Button::Update(float elapsed_time) {}
	void Button::Draw(sf::RenderTarget* target) {
		// Translate view to position
		sf::View view = target->getView();
		sf::Vector2u window_size = target->getSize();
		sf::Vector2f center = sf::Vector2f((float)window_size.x/2, (float)window_size.y/2);
		sf::Vector2f my_pos = GetPosition();
		target->setView(sf::View(center - GetPosition(), 2.f * center));

		sf::Vector2f size(rect_.width, rect_.height);
		sf::RectangleShape rect(size);
		rect.setOutlineThickness(2);

		switch (state_) {
		case NEUTRAL:
			rect.setFillColor(background_);
			rect.setOutlineColor(border_);
			break;
		case HOVERED:
			rect.setFillColor(hoverbackground_);
			rect.setOutlineColor(hover_border_);
			break;
		case PRESSED:
			rect.setFillColor(pressedbackground_);
			rect.setOutlineColor(pressed_border_);
			break;
		}
		
		target->draw(rect);
		if (texture_ != NULL) {
			sf::Sprite sprite;
			sprite.setTexture(*texture_);
			sprite.setPosition((size - sf::Vector2f(texture_->getSize())) * 0.5f);
			target->draw(sprite);
		}
		sf::Text text(string_);
		text.setColor(sf::Color::Black);
		sf::FloatRect text_size = text.getLocalBounds();
		text.setPosition((size - sf::Vector2f(text_size.width, text_size.height)) * 0.5f);
		target->draw(text);

		// Reset translation
		target->setView(view);
	}

	void Button::ProcessEvent(Event event) {
		switch (event.type) {
		case Event::E_MOUSE_RELEASED:
			if (state_ == PRESSED) {
				// Send a click event to listeners
				event.type = Event::E_CLICKED;
				Game::GetInstance()->GetEventManager()->AddEvent(event);
			}
		case Event::E_MOUSE_ENTERED:
			state_ = HOVERED;
			break;
		case Event::E_MOUSE_EXITED:
			state_ = NEUTRAL;
			break;
		case Event::E_MOUSE_PRESSED:
			state_ = PRESSED;
			break;
		}
	}

}
