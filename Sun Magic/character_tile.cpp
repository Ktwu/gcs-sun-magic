#include "stdafx.h"
#include "character_tile.h"
#include "game.h"
#include "sm_mouse.h"
#include "time.h"
#include "util.h"
#include <iterator>


namespace SunMagic {
	
zinnia::Recognizer *CharacterTile::_recognizer = NULL;

void CharacterTile::InitRecognizer(const char *modelFile) {
	_recognizer = zinnia::Recognizer::create();
	if (!_recognizer->open(modelFile)) {
		throw std::sprintf("ERROR: %s", _recognizer->what());
	}
}

sf::String CharacterTile::UTF8ToUTF32(const char* utf8str) {
	sf::Uint32 utf32str[] = {0,0};
	sf::Utf8::toUtf32((unsigned char*)utf8str, (unsigned char*)utf8str + strlen(utf8str), utf32str);
	return sf::String(utf32str);
}

CharacterTile::CharacterTile(float x, float y, size_t width, size_t height) :
	GameObject(x, y),
	_character(zinnia::Character::create()),
	_unicode(),
	_currentStroke(0),
	_traceCharacter(NULL),
	_traceUnicode(),
	_animatingStroke(-1),
	_animationSpeed(140.0f),
	_waitSeconds(0.2f),
	_strokeThickness(4),
	_borderColor(sf::Color(80, 80, 80)),
	_strokeColor(sf::Color::Black),
	_guideColor(sf::Color(238, 238, 238)),
	_traceColor(sf::Color(210, 210, 210)),
	_animateColor(sf::Color(150, 150, 150)),
	_secondsToWait(0),
	_currentLineDistance(0),
	_isWriting(false)
{
	_character->clear();
	_character->set_width(width);
	_character->set_height(height);
	
	_strokeLines.push_back(std::vector<sf::RectangleShape>());
}

CharacterTile::~CharacterTile() {
	delete _character;
}

zinnia::Character * CharacterTile::GetCharacter() {
	return _character;
}
sf::String CharacterTile::GetUnicode() {
	return _unicode;
}

size_t CharacterTile::NumStrokes() {
	return _character->strokes_size();
}
void CharacterTile::AddStrokePoint(sf::Vector2f p) {
	p -= _position;
	_character->add(_currentStroke, (int)p.x, (int)p.y);
	size_t points = _character->stroke_size(_currentStroke);
	if (points > 1) {
		sf::RectangleShape rect;
		sf::Vector2f prev((float)_character->x(_currentStroke, points-2),
			(float)_character->y(_currentStroke, points-2));
		sf::SetToLine(rect, prev, p, _strokeThickness);
		rect.setFillColor(_strokeColor);
		_strokeLines.back().push_back(rect);
	}
}
// Consider every two consecutive lines. If the angle between the lines is small enough remove the mid point.
size_t SmoothPoints(std::vector<sf::Vector2f>& points1, std::vector<sf::Vector2f>& points2, size_t index) {
	static const float THRESHOLD_ANGLE = cos(sfm::DEGTORAD(15));

	size_t i;
	size_t removed = 0;
	for (i = 0; i < index; i++) {
		points2.push_back(points1[i]);
	}
	for (; i + 2 < points1.size(); i += 2) {
		sf::Vector2f p1 = points1[i];
		sf::Vector2f p2 = points1[i+1];
		sf::Vector2f p3 = points1[i+2];

		sf::Vector2f diff1 = p2 - p1;
		sf::Vector2f diff2 = p3 - p2;

		float diffCosAngle = sfm::Dot(diff1, diff2) / (sfm::Length(diff1) * sfm::Length(diff2));

		points2.push_back(p1);
		if (diffCosAngle < THRESHOLD_ANGLE) {
			points2.push_back(p2);
		} else {
			removed++;
		}
	}
	for (; i < points1.size(); i++) {
		points2.push_back(points1[i]);
	}
	return removed;
}
void CharacterTile::EndStroke() {
	// Remove all lines of the last stroke
	_strokeLines.pop_back();

	// Copy points except for last stroke
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

	// Convert all points to Vector2f
	size_t num_points = _character->stroke_size(_currentStroke);
	std::vector<sf::Vector2f> points1(num_points);
	std::vector<sf::Vector2f> points2;
	for (size_t i = 0; i < num_points; i++) {
		points1[i] = sf::Vector2f((float)_character->x(_currentStroke, i), (float)_character->y(_currentStroke, i));
	}

	// Iteratively smooth the points. The smoothing only runs on every two lines,
	// so we do it twice with different indices to cover all points.
	size_t removed;
	do {
		removed = SmoothPoints(points1, points2, 0);
		points1.clear();
		removed += SmoothPoints(points2, points1, 1);
		points2.clear();
	} while (removed > 0);

	for (size_t i = 0; i < points1.size(); i++) {
		sf::Vector2f p = points1[i];
		character->add(_currentStroke, (int)p.x, (int)p.y);
	}

	delete _character;
	_character = character;

	// Add lines for the new stroke
	CreateLines(_strokeLines, _character, _strokeColor, _currentStroke, _currentStroke+1);
	_strokeLines.push_back(std::vector<sf::RectangleShape>());

	_currentStroke++;
	Reclassify();
}
void CharacterTile::UndoStroke() {
	if (_currentStroke < 0)
		return;
	
	// There is no remove stroke, so we just create a copy up to the last stroke.
	size_t strokes =_character->strokes_size();	
	if (_currentStroke == 0 && strokes == 0)
		return;

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
		_strokeLines.pop_back();
	} else {
	}
	_strokeLines.back().clear();
}
void CharacterTile::Clear() {
	_character->clear();
	_currentStroke = 0;
	_strokeLines.clear();
	_strokeLines.push_back(std::vector<sf::RectangleShape>());
	Reclassify();
}
float CharacterTile::GetStrokeError(size_t stroke) {
	if (stroke >= _character->strokes_size() || _traceCharacter == NULL ||
		stroke >= _traceCharacter->strokes_size()) {
			throw "Invalid stroke";
	}

	size_t i = 0;
	size_t j = 0;
	size_t points = _character->stroke_size(stroke);
	size_t trace_points = _traceCharacter->stroke_size(stroke);
	if (points < 2 || trace_points < 2) {
		// Not enough points to calculate error
		return 0;
	}

	float error = 0;
	sf::Vector2i p1 = sf::Vector2i(_character->x(stroke, 0), _character->y(stroke, 0));
	sf::Vector2i p2 = sf::Vector2i(_character->x(stroke, 1), _character->y(stroke, 1));
	sf::Vector2i gp1 = sf::Vector2i(_traceCharacter->x(stroke, 0), _traceCharacter->y(stroke, 0));
	sf::Vector2i gp2 = sf::Vector2i(_traceCharacter->x(stroke, 1), _traceCharacter->y(stroke, 1));
	while (i < points) {
		// Find the distance from the 
	}
	return 0;
}
void CharacterTile::Resize(size_t width, size_t height) {
	size_t curr_width = _character->width();
	size_t curr_height = _character->height();
	
	zinnia::Character *character = zinnia::createCharacter();
	character->set_width(width);
	character->set_height(height);
	for (int i = 0; i < _currentStroke; i++) {
		size_t points = _character->stroke_size(i);
		for (size_t j = 0; j < points; j++) {
			character->add(i, _character->x(i, j) * width / curr_width, _character->y(i, j) * height / curr_height);
		}
	}
}

zinnia::Character * CharacterTile::GetTraceCharacter() {
	return _traceCharacter;
}
sf::String CharacterTile::GetTraceUnicode() {
	return _traceUnicode;
}
void CharacterTile::SetTraceCharacter(zinnia::Character *character, sf::String value) {
	_traceCharacter = character;
	_traceUnicode = value;
	_animatingStroke = -1;
	
	_traceLines.clear();
	if (_traceCharacter != NULL) {
		CreateLines(_traceLines, _traceCharacter, _traceColor, 0, _traceCharacter->strokes_size());
	}
}

bool CharacterTile::IsAnimating() {
	return _animatingStroke >= 0;
}
int CharacterTile::GetAnimationStroke() {
	return _animatingStroke;
}
void CharacterTile::SetAnimationStroke(int stroke) {
	if (stroke == _animatingStroke)
		return;

	if (stroke >= 0) {
		if (_traceCharacter == NULL)
			throw "Trace character is NULL";
		if (stroke >= (int)_traceCharacter->strokes_size()) {
			stroke = -1;
		}
		_currentLineDistance = 0;
		_secondsToWait = 0;
		_animatingLines.clear();
	}
	_animatingStroke = stroke;
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

sf::Color CharacterTile::GetGuideColor() {
	return _guideColor;
}
void CharacterTile::SetGuideColor(sf::Color color) {
	_guideColor = color;
}

sf::Color CharacterTile::GetTraceColor() {
	return _traceColor;
}
void CharacterTile::SetTraceColor(sf::Color color) {
	_traceColor = color;
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
	// First thing's first, deal with input
	HandleInput();

	if (IsAnimating()) {
		// We wait a short while after finishing each stroke
		if (_secondsToWait > 0) {
			_secondsToWait -= elapsedSeconds;
			if (_secondsToWait <= 0) {
				// Reset animation stroke
				_currentLineDistance = 0;
				_animatingLines.clear();
			}
		} else {
			_currentLineDistance += elapsedSeconds * _animationSpeed;

			if (_animatingLines.size() > 0) {
				_animatingLines.pop_back();
			}
			sf::RectangleShape rect;
			rect.setFillColor(_animateColor);
			sf::Vector2f p2((float)_traceCharacter->x(_animatingStroke, _animatingLines.size()),
				(float)_traceCharacter->y(_animatingStroke, _animatingLines.size()));
			while (true) {
				if (_animatingLines.size() + 1 >= _traceCharacter->stroke_size(_animatingStroke)) {
					// Finished animating the stroke
					_secondsToWait = _waitSeconds;
					break;
				}

				sf::Vector2f p1 = p2;
				p2 = sf::Vector2f((float)_traceCharacter->x(_animatingStroke, _animatingLines.size() + 1),
					(float)_traceCharacter->y(_animatingStroke, _animatingLines.size() + 1));
				sf::Vector2f diff = p2 - p1;
				float distance = sfm::Length(diff);
				if (_currentLineDistance < distance) {
					float ratio = _currentLineDistance / distance;
					sf::Vector2f p = p1 + ratio * diff;
					sf::SetToLine(rect, p1, p, _strokeThickness);
					_animatingLines.push_back(rect);
					break;
				} else {
					_currentLineDistance -= distance;
					sf::SetToLine(rect, p1, p2, _strokeThickness);
					_animatingLines.push_back(rect);
				}
			}
		}
	}
}

void CharacterTile::HandleInput() {
	if (!_isWriting &&
	Game::mouse.haveButtonPressEvent && 
	Game::mouse.buttonPressEvent.button == sf::Mouse::Left) {
		std::cout << "start\n";
		_isWriting = true;
		int stroke = NumStrokes();
		
		_lastMouse.x = Game::mouse.buttonPressEvent.x;
		_lastMouse.y = Game::mouse.buttonPressEvent.y;
		AddStrokePoint(_lastMouse);
		std::cout << "Start Stroke (" << std::dec << _lastMouse.x << "," << _lastMouse.y << ")" << std::endl;
		return;
	}

	if (_isWriting &&
	Game::mouse.haveMoveEvent &&
	sf::squaredDistance(sf::Vector2f(Game::mouse.moveEvent.x, Game::mouse.moveEvent.y), _lastMouse) > MIN_STROKE_DISPLACEMENT_SQUARED) {
		std::cout << "stroke\n";
		_lastMouse.x = Game::mouse.moveEvent.x;
		_lastMouse.y = Game::mouse.moveEvent.y;	

		int stroke = NumStrokes() - 1;
		AddStrokePoint(_lastMouse);
		//std::cout << "Add stroke segment " << _character->stroke_size(stroke) << std::endl;
		return;	
	}
	
	if (_isWriting &&
	Game::mouse.haveButtonReleaseEvent &&
	Game::mouse.buttonReleaseEvent.button == sf::Mouse::Left) {
		std::cout << "release\n";
		_isWriting = false;
		EndStroke();
		SetAnimationStroke(NumStrokes());
		std::cout << "End Stroke" << std::endl;
		return;
		//updateText();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
		std::cout << "Clear Strokes" << std::endl;
		_isWriting = false;
		Clear();
		SetAnimationStroke(0);
		//updateText();
	} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
		std::cout << "Undo Stroke" << std::endl;
		if (_isWriting) {
			_isWriting = false;
			EndStroke();
		}
		UndoStroke();
		SetAnimationStroke(NumStrokes());
		//UpdateText();
	} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
		//_gameState = Game::Exiting;
	}
}

void CharacterTile::Draw(sf::RenderWindow *mainWindow) {
	sf::View view = mainWindow->getView();
	sf::Vector2f center = view.getCenter();
	mainWindow->setView(sf::View(center - _position, 2.f * center));

	float width = (float)_character->width();
	float height = (float)_character->height();

	// Draw background
	sf::RectangleShape rect(sf::Vector2f(width, height));
	rect.setFillColor(sf::Color::White);
	mainWindow->draw(rect);

	// Draw border lines
	rect.setFillColor(_borderColor);
	rect.setSize(sf::Vector2f(width, _strokeThickness));
	rect.setPosition(0, 0);
	mainWindow->draw(rect);
	rect.setPosition(0, height - _strokeThickness);
	mainWindow->draw(rect);
	rect.setSize(sf::Vector2f(_strokeThickness, height));
	rect.setPosition(0, 0);
	mainWindow->draw(rect);
	rect.setPosition(width - _strokeThickness, 0);
	mainWindow->draw(rect);

	// Draw guide lines
	rect.setFillColor(_guideColor);
	rect.setPosition(_strokeThickness, (height-_strokeThickness)/2);
	rect.setSize(sf::Vector2f(width - 2*_strokeThickness, _strokeThickness));
	mainWindow->draw(rect);
	rect.setPosition((width-_strokeThickness)/2, _strokeThickness);
	rect.setSize(sf::Vector2f(_strokeThickness, height - 2*_strokeThickness));
	mainWindow->draw(rect);

	sf::CircleShape circle(0.5f * _strokeThickness);
	circle.setOrigin(0.5f * _strokeThickness, 0.5f * _strokeThickness);
	if (_traceCharacter != NULL) {
		// Draw trace character up to the currently animated stroke, or all strokes if no animation
		size_t maxStroke = (_animatingStroke < 0) ? _traceLines.size() : std::min((int)_traceLines.size(), _animatingStroke + 1);		
		circle.setFillColor(_traceColor);
		for (size_t i = 0; i < maxStroke; i++) {
			circle.setPosition((float)_traceCharacter->x(i, 0), (float)_traceCharacter->y(i, 0));
			mainWindow->draw(circle);
			for (size_t j = 0; j < _traceLines[i].size(); j++) {
				mainWindow->draw(_traceLines[i][j]);
				circle.setPosition((float)_traceCharacter->x(i, j+1), (float)_traceCharacter->y(i, j+1));
				mainWindow->draw(circle);
			}
		}

		// Draw animating stroke
		circle.setFillColor(_animateColor);
		if (_animatingLines.size() > 0) {
			for (size_t j = 0; j < _animatingLines.size(); j++) {
				mainWindow->draw(_animatingLines[j]);
				circle.setPosition((float)_traceCharacter->x(_animatingStroke, j), (float)_traceCharacter->y(_animatingStroke, j));
				mainWindow->draw(circle);
			}
		}
	}

	// Draw current character
	circle.setFillColor(_strokeColor);
	for (size_t i = 0; i < _strokeLines.size(); i++) {
		circle.setPosition((float)_character->x(i, 0), (float)_character->y(i, 0));
		mainWindow->draw(circle);
		for (size_t j = 0; j < _strokeLines[i].size(); j++) {
			mainWindow->draw(_strokeLines[i][j]);
			circle.setPosition((float)_character->x(i, j+1), (float)_character->y(i, j+1));
			mainWindow->draw(circle);
		}
	}
	
	mainWindow->setView(view);
}

void CharacterTile::Reclassify () {
	if (_currentStroke == 0) {
		_unicode = sf::String();
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

void CharacterTile::CreateLines(std::vector<std::vector<sf::RectangleShape>>& lines, zinnia::Character *character, sf::Color color, size_t startStroke, size_t endStroke) {
	sf::RectangleShape rect;
	rect.setFillColor(color);

	for (size_t i = startStroke; i < endStroke; i++) {
		sf::Vector2f p2((float)character->x(i, 0), (float)character->y(i, 0));

		std::vector<sf::RectangleShape> stroke;
		for (size_t j = 1; j < character->stroke_size(i); j++) {
			sf::Vector2f p1 = p2;
			p2 = sf::Vector2f((float)character->x(i, j), (float)character->y(i, j));
			sf::SetToLine(rect, p1, p2, _strokeThickness);
			stroke.push_back(rect);
		}
		lines.push_back(stroke);
	}
}

}