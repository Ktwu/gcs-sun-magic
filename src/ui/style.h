#pragma once

#include "stdafx.h"

namespace sun_magic {

	class Style {
	public:
		Style();
		~Style();

		Style* SetTextSize(int text_size);
		Style* SetTextColor(sf::Color text_color);
		Style* SetTextFont(sf::Font text_font);
		Style* SetTextStyle(sf::Text::Style text_style);

		Style* SetAllowHover(bool allow_hover);
		Style* SetAllowPress(bool allow_press);
	
		Style* SetNormalColor(sf::Color normal_color);
		Style* SetHoverColor(sf::Color hover_color);
		Style* SetPressColor(sf::Color press_color);

		Style* SetNormalBorderColor(sf::Color normal_border_color);
		Style* SetHoverBorderColor(sf::Color hover_border_color);
		Style* SetPressBorderColor(sf::Color press_border_color);

		Style* SetNormalSprite(sf::Sprite normal_sprite);
		Style* SetHoverSprite(sf::Sprite hover_sprite);
		Style* SetPressSprite(sf::Sprite press_press);

		sf::Font		text_font;
		int				text_size;
		sf::Color		text_color;
		sf::Text::Style text_style;

		bool			allow_hover;
		bool			allow_press;

		sf::Color		normal_color;
		sf::Color		hover_color;
		sf::Color		press_color;

		sf::Color		normal_border_color;
		sf::Color		hover_border_color;
		sf::Color		press_border_color;

		sf::Sprite		normal_sprite;
		sf::Sprite		hover_sprite;
		sf::Sprite		press_sprite;
	};

}