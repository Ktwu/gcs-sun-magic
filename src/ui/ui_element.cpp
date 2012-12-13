#include "stdafx.h"
#include "ui_element.h"

#include "assets/gameasset_manager.h"
#include "game.h"
#include "events/event_manager.h"
#include "references/refs.h"

namespace sun_magic {

	UiElement::UiElement(float x, float y, float width, float height, sf::String text) :
		GameObject(x, y, width, height),
		string_(text),
		state_(NEUTRAL)
	{
		style_.Clear();
	}

	UiElement::~UiElement() {
	}


	Style* UiElement::GetStyle() {
		return &style_;
	}

	sf::String UiElement::GetString() {
		return string_;
	}
	void UiElement::SetString(sf::String string) {
		string_ = string;
	}

	void UiElement::Register() {
		Game::GetInstance()->GetEventManager()->RegisterListener(Event::E_MOUSE_ENTERED, this, this);
		Game::GetInstance()->GetEventManager()->RegisterListener(Event::E_MOUSE_EXITED, this, this);
		Game::GetInstance()->GetEventManager()->RegisterListener(Event::E_MOUSE_PRESSED, this, this);
		Game::GetInstance()->GetEventManager()->RegisterListener(Event::E_MOUSE_RELEASED, this, this);
	}
	void UiElement::Unregister() {
		Game::GetInstance()->GetEventManager()->UnregisterListener(Event::E_MOUSE_ENTERED, this, this);
		Game::GetInstance()->GetEventManager()->UnregisterListener(Event::E_MOUSE_EXITED, this, this);
		Game::GetInstance()->GetEventManager()->UnregisterListener(Event::E_MOUSE_PRESSED, this, this);
		Game::GetInstance()->GetEventManager()->UnregisterListener(Event::E_MOUSE_RELEASED, this, this);
	}

	void UiElement::Update(float elapsed_time) {}
	void UiElement::Draw(sf::RenderTarget* target) {
		if (!style_.is_visible)
			return;

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
		
		target->draw(rect);
		target->draw(sprite);

		sf::Text text(string_);
		text.setCharacterSize(style_.text_size);
		text.setStyle(style_.text_style);
		text.setColor(style_.text_color);
		text.setFont(style_.text_font);

		sf::FloatRect text_size = text.getLocalBounds();
		sf::Vector2f position(style_.text_padding, style_.text_padding);

		switch (style_.text_horizontal_align) {
			case Style::TextAlignment::CENTER:
				position.x = (size.x - text_size.width) * 0.5f;
				break;
			case Style::TextAlignment::LARGER:
				position.x = size.x - text_size.width - style_.text_padding;
		}
		switch (style_.text_vertical_align) {
			case Style::TextAlignment::CENTER:
				position.y = (size.y - text_size.height) * 0.5f;
				break;
			case Style::TextAlignment::LARGER:
				position.y = size.y - text_size.height - style_.text_padding;
		}

		text.setPosition(position);
		target->draw(text);
	}

	void UiElement::ProcessEvent(Event event) {
		if (!style_.is_enabled)
			return;

		switch (event.type) {
		case Event::E_MOUSE_RELEASED:
			if (state_ == PRESSED) {
				// Send a click event to listeners
				event.type = Event::E_CLICKED;
				event.message = string_;
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

	UiElement* UiElement::InitButton(UiElement* button) {
		button->GetStyle()->SetAllowHover(true)->SetAllowPress(true)
			->SetNormalColor(sf::Color(230,230,230))->SetNormalBorderColor(sf::Color(50,50,50))
			->SetHoverColor(sf::Color(200,200,200))->SetHoverBorderColor(sf::Color(100,100,100))
			->SetPressColor(sf::Color(150,150,150))->SetPressBorderColor(sf::Color(20,20,20))
			->SetTextSize(25)->SetTextStyle(sf::Text::Style::Regular)->SetTextColor(sf::Color::Black);
		return button;
	}
	UiElement* UiElement::InitLabel(UiElement* label){
		InitButton(label)->GetStyle()->SetAllowHover(false)->SetAllowPress(false);
		return label;
	}

}
