#include "stdafx.h"
#include "dictionary.h"

#include "game.h"
#include "assets/gameasset_manager.h"
#include "events/event_manager.h"
#include "references/refs.h"
#include "tools/sfm.h"
#include "tools/tools.h"

namespace sun_magic {

	Dictionary::Dictionary(float hide_x, float hide_y, float show_x, float show_y, float width, float height, bool is_vertical) : 
		//GameObject(hide_x,hide_y,width,height),
		GameObject(show_x, show_y, width, height),
		hide_pos_(sf::Vector2f(hide_x, hide_y)),
		show_pos_(sf::Vector2f(show_x, show_y)),
		is_vertical_(is_vertical),
		move_animation_(this, .3f)
	{
		font = GameAssetManager::GetInstance()->GetFont(this, refs::fonts::KAORI);
		english_font = GameAssetManager::GetInstance()->GetFont(this, refs::fonts::MSMINCHO);
	}
	Dictionary::~Dictionary() {
		GameAssetManager::GetInstance()->ReturnFonts(this);
	}

	void Dictionary::AddWord(sf::String word) {
		GameAssetManager *asset_manager = GameAssetManager::GetInstance();
		sf::Texture *texture = asset_manager->GetTexture(this, refs::textures::objects::SPRITES_MEH);
		sf::Sprite sprite = asset_manager->GetHiraganaSprite(word, texture);
		sprite.setScale(0.8f, 0.8f);

		int index = asset_manager->GetHiraganaIndex(word);
		DictionaryEntry entry;
		entry.outline = GameAssetManager::symbols_colors[index];
		entry.sprite = sprite;
		entry.romanji = GameAssetManager::romaji_strings[index];
		entry.tile = new CharacterTile(0, 0, 128, 128);
		entry.tile->SetZ(8.1f);
		entry.tile->SetWritable(false);
		entry.tile->SetTraceCharacter(asset_manager->GetTraceCharacter(word));
		entry.tile->SetAnimatingContinuously(true);
		entry.tile->GetStyle()->SetNormalBorderColor(sf::Color(190, 190, 190));
		entries_.push_back(entry);
	}

	void Dictionary::Clear() {
		entries_.clear();
	}

	void Dictionary::Register() {
		EventManager* event_manager = Game::GetInstance()->GetEventManager();
		event_manager->RegisterListener(Event::E_MOUSE_ENTERED, this, this);
		event_manager->RegisterListener(Event::E_MOUSE_EXITED, this, this);

		for (std::vector<DictionaryEntry>::iterator iter = entries_.begin(); iter != entries_.end(); iter++) {
			event_manager->AddGameObject(iter->tile);
		}
	}
	void Dictionary::Unregister() {
		EventManager* event_manager = Game::GetInstance()->GetEventManager();
		event_manager->UnregisterListener(Event::E_MOUSE_ENTERED, this, this);
		event_manager->UnregisterListener(Event::E_MOUSE_EXITED, this, this);

		for (std::vector<DictionaryEntry>::iterator iter = entries_.begin(); iter != entries_.end(); iter++) {
			event_manager->RemoveGameObject(iter->tile);
		}
	}

	void Dictionary::Update(float elapsed_time) {
		//move_animation_.Update(elapsed_time);
	}
	void Dictionary::Draw(sf::RenderTarget *target) {
		sf::Vector2f size = GetSize();

		sf::RectangleShape rect(size);
		rect.setFillColor(sf::Color::White);
		rect.setOutlineColor(sf::Color::Black);
		rect.setOutlineThickness(1);
		target->draw(rect);

		const float padding = 10;
		const float packing_ratio = 0.9f;
		float x = padding;
		float y = padding;

		sf::Text text;
		text.setFont(*english_font);
		text.setColor(sf::Color::Blue);
		text.setCharacterSize(25);
		text.setPosition(x, y);

		if (is_vertical_) {
			text.setString(sf::String("D\nI\nC\nT\nI\nO\nN\nA\nR\nY"));
			x += text.getGlobalBounds().width + padding;
		} else {
			text.setString(sf::String("DICTIONARY"));
			y += text.getGlobalBounds().height + padding;
		}
		target->draw(text);

		sf::Vector2f pos = GetPosition();
		for (std::vector<DictionaryEntry>::iterator iter = entries_.begin(); iter != entries_.end(); iter++) {
			sf::Sprite sprite = iter->sprite;
			sprite.setPosition(x, y);
			target->draw(sprite);
			sf::FloatRect sprite_bounds = sprite.getGlobalBounds();

			sf::Text text(iter->romanji);
			text.setColor(iter->outline);
			text.setCharacterSize(50);

			CharacterTile *tile = iter->tile;
			if (is_vertical_) {
				tile->SetPosition(pos + sf::Vector2f(x + padding + packing_ratio * sprite_bounds.width, y));

				text.setPosition(x + padding + packing_ratio * (sprite_bounds.width + tile->GetSize().x), y + sprite_bounds.height/2 - text.getLocalBounds().height/2);
				y += std::max(std::max(sprite_bounds.height, tile->GetSize().y), text.getGlobalBounds().height) + padding;
			} else {
				tile->SetPosition(pos + sf::Vector2f(x, y + padding + packing_ratio * sprite_bounds.height));

				text.setPosition(x + sprite_bounds.width/2 - text.getGlobalBounds().width/2, y + padding + packing_ratio * (sprite_bounds.height + tile->GetSize().y));
				x += std::max((sprite_bounds.width, tile->GetSize().x), text.getGlobalBounds().width) + padding;
			}

			target->draw(text);
		}
	}

	void Dictionary::ProcessEvent(Event event) {
		switch (event.type) {
		case Event::E_MOUSE_ENTERED:
			move_animation_.SetEnd(show_pos_);
			break;
		case Event::E_MOUSE_EXITED:
			move_animation_.SetEnd(hide_pos_);
			break;
		}
	}
}
