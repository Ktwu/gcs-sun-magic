#pragma once

#include "stdafx.h"
#include "events/event.h"
#include "ui/character_tile_style.h"
#include "ui/ui_element.h"

namespace sun_magic {

	class CharacterTile : public UiElement {
	public:
		// This must be called before using any Character Tiles
		static void InitRecognizer(const char *modelFile);
		static zinnia::Recognizer* GetRecognizer();
		static sf::String UTF8ToUTF32(const char* utf8str);

		CharacterTile(float x = 0, float y = 0, float width = 300, float height = 300);
		~CharacterTile();

		CharacterTileStyle* GetTileStyle();

		bool IsWritable();
		void SetWritable(bool writable);

		// Get the character that the user has written so far
		zinnia::Character * GetCharacter();
		sf::String GetUnicode();

		size_t NumStrokes();
		void AddStrokePoint(sf::Vector2f p);
		void EndStroke();
		void UndoStroke();
		void Clear();
		float GetError();

		// The trace character is the character the user should be trying to write
		zinnia::Character * GetTraceCharacter();
		sf::String GetTraceUnicode();
		void SetTraceCharacter(zinnia::Character *character);
	
		// Setting the animation stroke to a valid stroke number will make the tile
		// continuously animate that stroke
		bool IsAnimating();
		bool IsAnimatingContinuously();
		void SetAnimatingContinuously(bool animate_continuously);
		int GetAnimationStroke();
		void SetAnimationStroke(int stroke);

		void Register();
		void Unregister();
		void Update(float elapsed_time);
		void ProcessEvent(Event event);
		void Draw(sf::RenderTarget* target);		

	private:
		static const int MIN_STROKE_DISPLACEMENT_SQUARED = 8 * 8;
		static zinnia::Recognizer *_recognizer;

		void Reclassify();
		void CreateLines(std::vector<std::vector<sf::RectangleShape>>& lines, zinnia::Character *character, sf::Color color, size_t startStroke, size_t endStroke);
		void CalcError(zinnia::Character *trace);
		
		bool writable_;
		zinnia::Character *character_;
		sf::String unicode_;
		int current_stroke_;

		bool animate_continuously_;
		zinnia::Character *trace_character_;
		sf::String trace_unicode_;
		int animating_stroke_;

		CharacterTileStyle tilestyle_;

		// Current animation variables
		sf::Vector2f last_mouse_;
		bool is_writing_;
		float seconds_to_wait_;
		float current_line_distance_;
		std::vector<std::vector<sf::RectangleShape>> stroke_lines_;
		std::vector<std::vector<sf::RectangleShape>> trace_lines_;
		std::vector<sf::RectangleShape> animating_lines_;
		float error_;
	};

}