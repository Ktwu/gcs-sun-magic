#pragma once

#include "stdafx.h"
#include "game_object.h"

#include <iostream>
#include <sstream>

namespace SunMagic {

class CharacterTile : GameObject {

public:
	// This must be called before using any Character Tiles
	static void InitRecognizer(const char *modelFile);
	static sf::Unicode::UTF32String UTF8ToUTF32(const char* utf8str);

	CharacterTile(float x = 0, float y = 0, size_t width = 300, size_t height = 300);
	~CharacterTile();

	// Get the character that the user has written so far
	zinnia::Character * GetCharacter();
	sf::Unicode::UTF32String GetUnicode();

	size_t NumStrokes();
	void AddStrokePoint(float x, float y);
	void EndStroke();
	void UndoStroke();
	void Clear();
	void Resize(size_t width, size_t height);
	float GetStrokeError(size_t stroke);

	// The guide character is the character the user should be trying to write
	zinnia::Character * GetGuideCharacter();
	sf::Unicode::UTF32String GetGuideUnicode();
	void SetGuideCharacter(zinnia::Character *character, sf::Unicode::UTF32String unicode);
	
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

	sf::Color GetGuideLineColor();
	void SetGuideLineColor(sf::Color color);

	sf::Color GetGuideColor();
	void SetGuideColor(sf::Color color);

	sf::Color GetAnimateColor();
	void SetAnimateColor(sf::Color color);

	sf::Color GetStrokeColor();
	void SetStrokeColor(sf::Color color);

	void Update(float elapsedSeconds);
	void Draw(sf::RenderWindow *mainWindow);

private:
	static zinnia::Recognizer *_recognizer;

	void Reclassify();
	void DrawStrokes(sf::RenderWindow *mainWindow, zinnia::Character *character, sf::Color color, size_t startStroke, size_t endStroke);

	zinnia::Character *_character;
	sf::Unicode::UTF32String _unicode;
	int _currentStroke;

	zinnia::Character *_guideCharacter;
	sf::Unicode::UTF32String _guideUnicode;
	int _animationStroke;
	float _animationSpeed;
	float _waitSeconds;

	float _strokeThickness;
	sf::Color _borderColor;
	sf::Color _guideLineColor;
	sf::Color _guideColor;
	sf::Color _animateColor;
	sf::Color _strokeColor;

	// Current animation variables
	float _secondsToWait;
	float _currentLineDistance;
	std::vector<sf::Shape> _animationStrokeLines;
};

}