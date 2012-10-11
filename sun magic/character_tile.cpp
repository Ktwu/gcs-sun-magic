#include "stdafx.h"
#include "character_tile.h"

#include <fstream>
#include <iostream>

#include "time.h"

namespace SunMagic {
	
zinnia::Recognizer *CharacterTile::_recognizer = NULL;

void CharacterTile::InitRecognizer(const char *modelFile) {
	_recognizer = zinnia::Recognizer::create();
	if (!_recognizer->open(modelFile)) {
		throw std::sprintf("ERROR: %s", _recognizer->what());
	}
}

sf::Unicode::UTF32String CharacterTile::UTF8ToUTF32(const char* utf8str) {
	std::basic_string<sf::Uint32> utf32str;
	sf::Unicode::UTF8ToUTF32((unsigned char*)utf8str, (unsigned char*)utf8str + strlen(utf8str), back_inserter(utf32str));
	return utf32str;
}

CharacterTile::CharacterTile(float x, float y, size_t width, size_t height) :
	GameObject(x, y),
	_character(zinnia::Character::create()),
	_unicode(),
	_currentStroke(0),
	_guideCharacter(NULL),
	_guideUnicode(),
	_animationStroke(-1),
	_animationSpeed(140.0f),
	_waitSeconds(0.2f),
	_strokeThickness(4),
	_borderColor(sf::Color(80, 80, 80)),
	_strokeColor(sf::Color::Black),
	_guideLineColor(sf::Color(238, 238, 238)),
	_guideColor(sf::Color(210, 210, 210)),
	_animateColor(sf::Color(150, 150, 150)),
	_secondsToWait(0),
	_currentLineDistance(0)
{
	_character->clear();
	_character->set_width(width);
	_character->set_height(height);
}

CharacterTile::~CharacterTile() {
	delete _character;
}

zinnia::Character * CharacterTile::GetCharacter() {
	return _character;
}
sf::Unicode::UTF32String CharacterTile::GetUnicode() {
	return _unicode;
}

size_t CharacterTile::NumStrokes() {
	return _character->strokes_size();
}
void CharacterTile::AddStrokePoint(float x, float y) {
	_character->add(_currentStroke, x - _position.x, y - _position.y);
}
void CharacterTile::EndStroke() {
	_currentStroke++;
	Reclassify();
}
void CharacterTile::UndoStroke() {
	if (_currentStroke < 0)
		return;

	// There is no remove stroke, so we just create a copy up to the last stroke.
	size_t strokes =_character->strokes_size();
	zinnia::Character *character = zinnia::createCharacter();
	character->set_width(_character->width());
	character->set_height(_character->height());
	for (size_t i = 0; i < strokes - 1; i++) {
		size_t points = _character->stroke_size(i);
		for (size_t j = 0; j < points; j++) {
			character->add(i, _character->x(i, j), _character->y(i, j));
		}
	}

	delete _character;
	_character = character;
	
	if (_currentStroke == strokes) {
		_currentStroke--;
		Reclassify();
	}
}
void CharacterTile::Clear() {
	_character->clear();
	_currentStroke = 0;
	Reclassify();
}
void CharacterTile::Resize(size_t width, size_t height) {
	size_t curr_width = _character->width();
	size_t curr_height = _character->height();
	
	zinnia::Character *character = zinnia::createCharacter();
	character->set_width(width);
	character->set_height(height);
	for (size_t i = 0; i < _currentStroke; i++) {
		size_t points = _character->stroke_size(i);
		for (size_t j = 0; j < points; j++) {
			character->add(i, _character->x(i, j) * width / curr_width, _character->y(i, j) * height / curr_height);
		}
	}
}

zinnia::Character * CharacterTile::GetGuideCharacter() {
	return _guideCharacter;
}
sf::Unicode::UTF32String CharacterTile::GetGuideUnicode() {
	return _guideUnicode;
}
void CharacterTile::SetGuideCharacter(zinnia::Character *character, sf::Unicode::UTF32String value) {
	_guideCharacter = character;
	_guideUnicode = value;
	_animationStroke = -1;
}

bool CharacterTile::IsAnimating() {
	return _animationStroke >= 0;
}
int CharacterTile::GetAnimationStroke() {
	return _animationStroke;
}
void CharacterTile::SetAnimationStroke(int stroke) {
	if (stroke >= 0) {
		if (_guideCharacter == NULL)
			throw "Guide character is NULL";
		if (stroke >= _guideCharacter->strokes_size()) {
			stroke = -1;
		}
		_currentLineDistance = 0;
		_secondsToWait = 0;
		_animationStrokeLines.clear();
	}
	_animationStroke = stroke;
}

float CharacterTile::GetAnimationSpeed() {
	return _animationSpeed;
}
void CharacterTile::SetAnimationSpeed(float speed) {
	_animationSpeed = speed;
}

float CharacterTile::GetAnimationWait() {
	return _waitSeconds;
}
void CharacterTile::SetAnimaitonWait(float secondsToWait) {
	_waitSeconds = secondsToWait;
}

float CharacterTile::GetStrokeThickness() {
	return _strokeThickness;
}
void CharacterTile::SetStrokeThickness(float thickness) {
	_strokeThickness = thickness;
}

sf::Color CharacterTile::GetBorderColor() {
	return _borderColor;
}
void CharacterTile::SetBorderColor(sf::Color color) {
	_borderColor = color;
}

sf::Color CharacterTile::GetGuideLineColor() {
	return _guideLineColor;
}
void CharacterTile::SetGuideLineColor(sf::Color color) {
	_guideLineColor = color;
}

sf::Color CharacterTile::GetGuideColor() {
	return _guideColor;
}
void CharacterTile::SetGuideColor(sf::Color color) {
	_guideColor = color;
}

sf::Color CharacterTile::GetAnimateColor() {
	return _animateColor;
}
void CharacterTile::SetAnimateColor(sf::Color color) {
	_animateColor = color;
}

sf::Color CharacterTile::GetStrokeColor() {
	return _strokeColor;
}
void CharacterTile::SetStrokeColor(sf::Color color) {
	_strokeColor = color;
}

void CharacterTile::Update(float elapsedSeconds) {
	if (IsAnimating()) {
		// We wait a short while after finishing each stroke
		if (_secondsToWait > 0) {
			_secondsToWait -= elapsedSeconds;
			if (_secondsToWait <= 0) {
				// Reset animation stroke
				_currentLineDistance = 0;
				_animationStrokeLines.clear();
			}
		} else {
			_currentLineDistance += elapsedSeconds * _animationSpeed;

			if (_animationStrokeLines.size() > 0) {
				_animationStrokeLines.pop_back();
			}
			float x2 = _guideCharacter->x(_animationStroke, _animationStrokeLines.size());
			float y2 = _guideCharacter->y(_animationStroke, _animationStrokeLines.size());
			while (true) {
				if (_animationStrokeLines.size() + 1 >= _guideCharacter->stroke_size(_animationStroke)) {
					// Finished animating the stroke
					_secondsToWait = _waitSeconds;
					break;
				}

				float x1 = x2;
				float y1 = y2;
				x2 = _guideCharacter->x(_animationStroke, _animationStrokeLines.size() + 1);
				y2 = _guideCharacter->y(_animationStroke, _animationStrokeLines.size() + 1);
				float dx = x2 - x1;
				float dy = y2 - y1;
				float distance = sqrt(dx * dx + dy * dy);
				if (_currentLineDistance < distance) {
					float ratio = _currentLineDistance / distance;
					float x = x1 + ratio * (x2 - x1);
					float y = y1 + ratio * (y2 - y1);
					_animationStrokeLines.push_back(sf::Shape::Line(x1, y1, x, y, _strokeThickness, _animateColor));
					break;
				} else {
					_currentLineDistance -= distance;
					_animationStrokeLines.push_back(sf::Shape::Line(x1, y1, x2, y2, _strokeThickness, _animateColor));
				}
			}
		}
	}
}

void CharacterTile::Draw(sf::RenderWindow *mainWindow) {
	const sf::View& view = mainWindow->GetView();
	mainWindow->SetView(sf::View(view.GetHalfSize() - _position, view.GetHalfSize()));

	float width = _character->width();
	float height = _character->height();
	// Draw background
	mainWindow->Draw(sf::Shape::Rectangle(sf::Vector2f(0,0), sf::Vector2f(width, height), sf::Color::White));

	// Draw grid lines
	float halfStroke = _strokeThickness / 2;
	float positionWidth = width - halfStroke;
	float positionHeight = height - halfStroke;
	// Draw border
	mainWindow->Draw(sf::Shape::Line(0, halfStroke, width, halfStroke, _strokeThickness, _borderColor));
	mainWindow->Draw(sf::Shape::Line(0, height, width, height, _strokeThickness, _borderColor));
	mainWindow->Draw(sf::Shape::Line(halfStroke, 0, halfStroke, height, _strokeThickness, _borderColor));
	mainWindow->Draw(sf::Shape::Line(positionWidth, 0, positionWidth, height, _strokeThickness, _borderColor));
	// Draw guide lines
	mainWindow->Draw(sf::Shape::Line(_strokeThickness, positionHeight/2, width - _strokeThickness, positionHeight/2, _strokeThickness, _guideLineColor));
	mainWindow->Draw(sf::Shape::Line(positionWidth/2, _strokeThickness, positionWidth/2, height - _strokeThickness, _strokeThickness, _guideLineColor));

	// Draw guide character
	if (_guideCharacter != NULL) {
		// Draw stroke animation
		if (IsAnimating()) {
			DrawStrokes(mainWindow, _guideCharacter, _animateColor, 0, _animationStroke);
			DrawStrokes(mainWindow, _guideCharacter, _guideColor, _animationStroke, _guideCharacter->strokes_size());
			for (int i = 0; i < _animationStrokeLines.size(); i++) {
				mainWindow->Draw(_animationStrokeLines[i]);
			}
		} else {
			DrawStrokes(mainWindow, _guideCharacter, _animateColor, 0, _guideCharacter->strokes_size());
		}
	}

	// Draw strokes of current character
	DrawStrokes(mainWindow, _character, _strokeColor, 0, _character->strokes_size());

	mainWindow->SetView(view);
}

void CharacterTile::Reclassify() {
	if (_currentStroke == 0) {
		_unicode = sf::Unicode::Text();
		return;
	}

	zinnia::Result *result = _recognizer->classify(*_character, 1);
	if (result) {
		std::cout << result->size() << std::endl;
		_unicode = UTF8ToUTF32(result->value(0));
	} else {
		throw std::sprintf("ERROR: %s", _recognizer->what());
	}
	delete result;
}

void CharacterTile::DrawStrokes(sf::RenderWindow *mainWindow, zinnia::Character *character, sf::Color color, size_t startStroke, size_t endStroke) {
	for (size_t i = startStroke; i < endStroke; i++) {
		float x2 = character->x(i, 0);
		float y2 = character->y(i, 0);
		for (size_t j = 1; j < character->stroke_size(i); j++) {
			float x1 = x2;
			float y1 = y2;
			x2 = character->x(i, j);
			y2 = character->y(i, j);
			mainWindow->Draw(sf::Shape::Line(x1, y1, x2, y2, _strokeThickness, color));
		}
	}
}

}