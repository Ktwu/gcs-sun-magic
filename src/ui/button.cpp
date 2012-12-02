#include "stdafx.h"
#include "button.h"

#include "assets/gameasset_manager.h"
#include "game.h"
#include "events/event_manager.h"
#include "references/refs.h"

namespace sun_magic {

	Button::Button(float x, float y, float width, float height, sf::String text) :
		GameObject(x, y, width, height),
		string_(text),
		state_(NEUTRAL)
	{
		style_.SetAllowHover(true)->SetAllowPress(true);
		style_.SetNormalColor(sf::Color(230,230,230))->SetNormalBorderColor(sf::Color(50,50,50));
		style_.SetHoverColor(sf::Color(200,200,200))->SetHoverBorderColor(sf::Color(100,100,100));
		style_.SetPressColor(sf::Color(150,150,150))->SetPressBorderColor(sf::Color(20,20,20));
		style_.SetTextFont(*GameAssetManager::GetInstance()->GetFont(refs::fonts::MSMINCHO));
	}

	Button::~Button() {}

	Style* Button::GetStyle() {
		return &style_;
	}

	sf::String Button::GetString() {
		return string_;
	}
	void Button::SetString(sf::String string) {
		string_ = string;
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
		sf::Vector2f size(rect_.width, rect_.height);
		sf::RectangleShape rect(size);
		sf::Sprite sprite;
		rect.setOutlineThickness(2);

		switch (state_) {
		case NEUTRAL:
			sprite = style_.normal_sprite;
			rect.setFillColor(style_.normal_color);
			rect.setOutlineColor(style_.normal_border_color);
			break;
		case HOVERED:
			sprite = style_.hover_sprite;
			rect.setFillColor(style_.hover_color);
			rect.setOutlineColor(style_.hover_border_color);
			break;
		case PRESSED:
			sprite = style_.press_sprite;
			rect.setFillColor(style_.press_color);
			rect.setOutlineColor(style_.press_border_color);
			break;
		}
		
		target->draw(sprite);
		target->draw(rect);

		sf::Text text(string_);
		text.setCharacterSize(style_.text_size);
		text.setStyle(style_.text_style);
		text.setColor(style_.text_color);
		text.setFont(style_.text_font);

		sf::FloatRect text_size = text.getLocalBounds();
		text.setPosition((size - sf::Vector2f(text_size.width, text_size.height)) * 0.5f);
		target->draw(text);
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
			if (style_.allow_hover)
				state_ = HOVERED;
			break;
		case Event::E_MOUSE_EXITED:
			state_ = NEUTRAL;
			break;
		case Event::E_MOUSE_PRESSED:
			if (style_.allow_press)
				state_ = PRESSED;
			break;
		}
	}

}
