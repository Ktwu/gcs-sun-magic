#pragma once

#include "stdafx.h"
#include "game_object.h"

namespace SunMagic {

	class CharacterTile : GameObject {

	public:
		// This must be called before using any Character Tiles
		static void InitRecognizer(const char *modelFile);
		static sf::String UTF8ToUTF32(const char* utf8str);

		CharacterTile(float x = 0, float y = 0, float width = 300, float height = 300);
		~CharacterTile();

		// Get the character that the user has written so far
		zinnia::Character * GetCharacter();
		sf::String GetUnicode();

		size_t NumStrokes();
		void AddStrokePoint(sf::Vector2f p);
		void EndStroke();
		void UndoStroke();
		void Clear();
		void Resize(float width, float height);
		float GetStrokeError(size_t stroke);

		// The trace character is the character the user should be trying to write
		zinnia::Character * GetTraceCharacter();
		sf::String GetTraceUnicode();
		void SetTraceCharacter(zinnia::Character *character, sf::String unicode);
	
		// Setting the animation stroke to a valid stroke number will make the tile
		// continuously animate that stroke
		bool IsAnimating();
		int GetAnimationStroke();
		void SetAnimationStroke(int stroke);

		float GetAnimationSpeed();
		void SetAnimationSpeed(float speed);

		float GetAnimationWait();
		void SetAnimaitonWait(float secondsToWait);

		float GetStrokeThickness();
		void SetStrokeThickness(float thickness);

		sf::Color GetBorderColor();
		void SetBorderColor(sf::Color color);

		sf::Color GetGuideColor();
		void SetGuideColor(sf::Color color);

		sf::Color GetTraceColor();
		void SetTraceColor(sf::Color color);

		sf::Color GetAnimateColor();
		void SetAnimateColor(sf::Color color);

		sf::Color GetStrokeColor();
		void SetStrokeColor(sf::Color color);

		void Update(float elapsedSeconds);
		void HandleInput();
		void Draw(sf::RenderWindow *mainWindow);

	private:
		static const int MIN_STROKE_DISPLACEMENT_SQUARED = 8 * 8;
		static zinnia::Recognizer *_recognizer;

		void Reclassify();
		void CreateLines(std::vector<std::vector<sf::RectangleShape>>& lines, zinnia::Character *character, sf::Color color, size_t startStroke, size_t endStroke);
		void CalcStrokeError();

		zinnia::Character *_character;
		sf::String _unicode;
		int _currentStroke;

		zinnia::Character *_traceCharacter;
		sf::String _traceUnicode;
		int _animatingStroke;
		float _animationSpeed;
		float _waitSeconds;

		float _strokeThickness;
		sf::Color _borderColor;
		sf::Color _guideColor;
		sf::Color _traceColor;
		sf::Color _animateColor;
		sf::Color _strokeColor;

		sf::Vector2f _lastMouse;
		sf::Vector2f _currMouse;

		// Current animation variables
		bool _isWriting;
		float _secondsToWait;
		float _currentLineDistance;
		std::vector<std::vector<sf::RectangleShape>> _strokeLines;
		std::vector<std::vector<sf::RectangleShape>> _traceLines;
		std::vector<sf::RectangleShape> _animatingLines;
		std::vector<float> _strokeErrors;
	};

}