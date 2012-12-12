#include "stdafx.h"
#include "style.h"

namespace sun_magic {
	Style::Style() {
		Clear();
	}

	Style::~Style() {}

	Style* Style::Clear() {
		text_size = 25;
		text_color = sf::Color::Transparent;
		text_style = sf::Text::Regular;
		text_padding = 0;
		text_horizontal_align = CENTER;
		text_vertical_align = CENTER;
		allow_hover = false;
		allow_press = false;
		normal_color = sf::Color::Transparent;
		hover_color = sf::Color::Transparent;
		press_color = sf::Color::Transparent;
		normal_border_color = sf::Color::Transparent;
		hover_border_color = sf::Color::Transparent;
		press_border_color = sf::Color::Transparent;
		normal_sprite = sf::Sprite();
		hover_sprite = sf::Sprite();
		press_sprite = sf::Sprite();
		text_font = sf::Font();
		return this;
	}

	Style* Style::SetTextSize(int text_size) {
		this->text_size = text_size;
		return this;
	}
	Style* Style::SetTextColor(sf::Color text_color) {
		this->text_color = text_color;
		return this;
	}
	Style* Style::SetTextFont(sf::Font font) {
		this->text_font = font;
		return this;
	}
	Style* Style::SetTextStyle(sf::Text::Style text_style) {
		this->text_style = text_style;
		return this;
	}
	Style* Style::SetTextPadding(float padding) {
		this->text_padding = padding;
		return this;
	}
	Style* Style::SetTextHorizontalAlignment(TextAlignment align) {
		this->text_horizontal_align = align;
		return this;
	}
	Style* Style::SetTextVerticalAlignment(TextAlignment align) {
		this->text_vertical_align = align;
		return this;
	}

	Style* Style::SetAllowHover(bool allow_hover) {
		this->allow_hover = allow_hover;
		return this;
	}
	Style* Style::SetAllowPress(bool allow_press) {
		this->allow_press = allow_press;
		return this;
	}
	
	Style* Style::SetNormalColor(sf::Color normal_color) {
		this->normal_color = normal_color;
		return this;
	}
	Style* Style::SetHoverColor(sf::Color hover_color) {
		this->hover_color = hover_color;
		return this;
	}
	Style* Style::SetPressColor(sf::Color press_color) {
		this->press_color = press_color;
		return this;
	}

	Style* Style::SetNormalBorderColor(sf::Color normal_border_color) {
		this->normal_border_color = normal_border_color;
		return this;
	}
	Style* Style::SetHoverBorderColor(sf::Color hover_border_color) {
		this->hover_border_color = hover_border_color;
		return this;
	}
	Style* Style::SetPressBorderColor(sf::Color press_border_color) {
		this->press_border_color = press_border_color;
		return this;
	}

	Style* Style::SetNormalSprite(sf::Sprite normal_sprite) {
		this->normal_sprite = normal_sprite;
		return this;
	}
	Style* Style::SetHoverSprite(sf::Sprite hover_sprite) {
		this->hover_sprite = hover_sprite;
		return this;
	}
	Style* Style::SetPressSprite(sf::Sprite press_sprite) {
		this->press_sprite = press_sprite;
		return this;
	}

}
