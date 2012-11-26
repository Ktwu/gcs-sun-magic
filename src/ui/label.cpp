#include "stdafx.h"
#include "label.h"

#include "assets/gameasset_manager.h"
#include "game.h"
#include "events/event_manager.h"

namespace sun_magic {

	Label::Label(float x, float y, float width, float height, sf::String text, sf::Texture *texture) :
		GameObject(x, y, width, height),
		string_(text),
		texture_(texture),
		background_(sf::Color(230,230,230)),
		border_(sf::Color(50,50,50))
	{
	}

	Label::~Label() {}

	sf::String Label::GetString() {
		return string_;
	}
	void Label::SetString(sf::String string) {
		string_ = string;
	}

	sf::Texture* Label::GetTexture() {
		return texture_;
	}
	void Label::SetTexture(sf::Texture *texture) {
		texture_ = texture;
	}

	void Label::Register() {}
	void Label::Unregister() {}

	void Label::Update(float elapsed_time) {}
	void Label::Draw(sf::RenderTarget* target) {
		sf::Vector2f size(rect_.width, rect_.height);
		sf::RectangleShape rect(size);
		rect.setOutlineThickness(2);
		rect.setOutlineColor(border_);

		target->draw(rect);
		if (texture_ != NULL) {
			sf::Sprite sprite;
			sprite.setTexture(*texture_);
			sprite.setPosition((size - sf::Vector2f(texture_->getSize())) * 0.5f);
			target->draw(sprite);
		}

		sf::Text text(string_);
		text.setFont(GameAssetManager::GetInstance()->GetMsminchoFont());
		text.setCharacterSize(40);
		text.setColor(sf::Color::Black);
		sf::FloatRect text_size = text.getLocalBounds();
		text.setPosition((size - sf::Vector2f(text_size.width, text_size.height)) * 0.5f);
		target->draw(text);
	}

}
