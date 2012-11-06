#include "stdafx.h"
#include "character_tile.h"
#include "game.h"
#include "events.h"
#include "time.h"
#include "util.h"
#include <iterator>

namespace sun_magic {
	
	zinnia::Recognizer *CharacterTile::_recognizer = NULL;

	void CharacterTile::InitRecognizer(const char *modelFile) {
		_recognizer = zinnia::Recognizer::create();
		if (!_recognizer->open(modelFile)) {
			throw "ERROR: " + std::string(_recognizer->what());
		}
	}

	zinnia::Recognizer* CharacterTile::GetRecognizer() {
		return _recognizer;
	}

	sf::String CharacterTile::UTF8ToUTF32(const char* utf8str) {
		sf::Uint32 utf32str[] = {0,0};
		sf::Utf8::toUtf32((unsigned char*)utf8str, (unsigned char*)utf8str + strlen(utf8str), utf32str);
		return sf::String(utf32str);
	}

	CharacterTile::CharacterTile(float x, float y, float width, float height) :
		GameObject(x, y, width, height),
		character_(zinnia::Character::create()),
		unicode_(),
		current_stroke_(0),
		trace_character_(NULL),
		trace_unicode_(),
		animating_stroke_(-1),
		animation_speed_(140.0f),
		wait_seconds_(0.2f),
		stroke_thickness_(4),
		border_color_(sf::Color(80, 80, 80)),
		stroke_color_(sf::Color::Black),
		guide_color_(sf::Color(238, 238, 238)),
		trace_color_(sf::Color(210, 210, 210)),
		animate_color_(sf::Color(150, 150, 150)),
		seconds_to_wait_(0),
		current_line_distance_(0),
		is_writing_(false)
	{
		character_->clear();
		character_->set_width((size_t)width);
		character_->set_height((size_t)height);
	
		stroke_lines_.push_back(std::vector<sf::RectangleShape>());
	}

	CharacterTile::~CharacterTile() {
		delete character_;
	}

	zinnia::Character * CharacterTile::GetCharacter() {
		return character_;
	}
	sf::String CharacterTile::GetUnicode() {
		return unicode_;
	}

	size_t CharacterTile::NumStrokes() {
		// If currently writing, don't count the stroke beign written
		return character_->strokes_size() - ((is_writing_) ? 1 : 0);
	}
	void CharacterTile::AddStrokePoint(sf::Vector2f p) {
		p -= GetPosition();
		character_->add(current_stroke_, (int)p.x, (int)p.y);
		size_t points = character_->stroke_size(current_stroke_);
		if (points > 1) {
			sf::RectangleShape rect;
			sf::Vector2f prev((float)character_->x(current_stroke_, points-2),
				(float)character_->y(current_stroke_, points-2));
			sf::SetToLine(rect, prev, p, stroke_thickness_);
			rect.setFillColor(stroke_color_);
			stroke_lines_.back().push_back(rect);
		}
	}
	// Consider every two consecutive lines. If the angle between the lines is small enough remove the mid point.
	size_t SmoothPoints(std::vector<sf::Vector2f>& points1, std::vector<sf::Vector2f>& points2, size_t index) {
		static const float THRESHOLD_ANGLE = cos(sfm::DEGTORAD(13));

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
		stroke_lines_.pop_back();

		// Copy points except for last stroke
		size_t strokes =character_->strokes_size();
		zinnia::Character *character = zinnia::createCharacter();
		character->set_width(character_->width());
		character->set_height(character_->height());
		for (size_t i = 0; i < strokes - 1; i++) {
			size_t points = character_->stroke_size(i);
			for (size_t j = 0; j < points; j++) {
				character->add(i, character_->x(i, j), character_->y(i, j));
			}
		}

		// Convert all points to Vector2f
		size_t num_points = character_->stroke_size(current_stroke_);
		std::vector<sf::Vector2f> points1(num_points);
		std::vector<sf::Vector2f> points2;
		for (size_t i = 0; i < num_points; i++) {
			points1[i] = sf::Vector2f((float)character_->x(current_stroke_, i), (float)character_->y(current_stroke_, i));
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
			character->add(current_stroke_, (int)p.x, (int)p.y);
		}

		delete character_;
		character_ = character;

		// Add lines for the new stroke
		CreateLines(stroke_lines_, character_, stroke_color_, current_stroke_, current_stroke_+1);
		stroke_lines_.push_back(std::vector<sf::RectangleShape>());

		current_stroke_++;
		Reclassify();
		if (trace_character_ != NULL) {
			CalcStrokeError();
		}
	}
	void CharacterTile::UndoStroke() {
		if (current_stroke_ < 0)
			return;
	
		// There is no remove stroke, so we just create a copy up to the last stroke.
		size_t strokes =character_->strokes_size();	
		if (current_stroke_ == 0 && strokes == 0)
			return;

		zinnia::Character *character = zinnia::createCharacter();
		character->set_width(character_->width());
		character->set_height(character_->height());
		for (size_t i = 0; i < strokes - 1; i++) {
			size_t points = character_->stroke_size(i);
			for (size_t j = 0; j < points; j++) {
				character->add(i, character_->x(i, j), character_->y(i, j));
			}
		}

		delete character_;
		character_ = character;
	
		if (current_stroke_ == strokes) {
			current_stroke_--;
			Reclassify();
			stroke_lines_.pop_back();
			stroke_errors_.pop_back();
		} else {
		}
		stroke_lines_.back().clear();
	}
	void CharacterTile::Clear() {
		character_->clear();
		current_stroke_ = 0;
		stroke_lines_.clear();
		stroke_lines_.push_back(std::vector<sf::RectangleShape>());
		stroke_errors_.clear();
		Reclassify();
	}
	float CharacterTile::GetStrokeError(size_t stroke) {
		return stroke_errors_[stroke];
	}
	void CharacterTile::Resize(float width, float height) {
		sf::Vector2f size = GetSize();
	
		zinnia::Character *character = zinnia::createCharacter();
		character->set_width((size_t)width);
		character->set_height((size_t)height);
		for (int i = 0; i < current_stroke_; i++) {
			size_t points = character_->stroke_size(i);
			for (size_t j = 0; j < points; j++) {
				character->add(i, (int)(character_->x(i, j) * width / size.x), (int)(character_->y(i, j) * height / size.y));
			}
		}
		_rect.width = width;
		_rect.height = height;
	}

	zinnia::Character * CharacterTile::GetTraceCharacter() {
		return trace_character_;
	}
	sf::String CharacterTile::GetTraceUnicode() {
		return trace_unicode_;
	}
	void CharacterTile::SetTraceCharacter(zinnia::Character *character, sf::String value) {
		trace_character_ = character;
		trace_unicode_ = value;
		animating_stroke_ = -1;
	
		trace_lines_.clear();
		if (trace_character_ != NULL) {
			CreateLines(trace_lines_, trace_character_, trace_color_, 0, trace_character_->strokes_size());
		}
	}

	bool CharacterTile::IsAnimating() {
		return animating_stroke_ >= 0;
	}
	int CharacterTile::GetAnimationStroke() {
		return animating_stroke_;
	}
	void CharacterTile::SetAnimationStroke(int stroke) {
		if (stroke == animating_stroke_ || trace_character_ == NULL)
			return;

		if (stroke >= 0) {
			if (stroke >= (int) trace_character_->strokes_size()) {
				stroke = -1;
			}
			current_line_distance_ = 0;
			seconds_to_wait_ = 0;
			animating_lines_.clear();
		}
		animating_stroke_ = stroke;
	}

	float CharacterTile::GetAnimationSpeed() {
		return animation_speed_;
	}
	void CharacterTile::SetAnimationSpeed(float speed) {
		animation_speed_ = speed;
	}

	float CharacterTile::GetAnimationWait() {
		return wait_seconds_;
	}
	void CharacterTile::SetAnimaitonWait(float secondsToWait) {
		wait_seconds_ = secondsToWait;
	}

	float CharacterTile::GetStrokeThickness() {
		return stroke_thickness_;
	}
	void CharacterTile::SetStrokeThickness(float thickness) {
		stroke_thickness_ = thickness;
	}

	sf::Color CharacterTile::GetBorderColor() {
		return border_color_;
	}
	void CharacterTile::SetBorderColor(sf::Color color) {
		border_color_ = color;
	}

	sf::Color CharacterTile::GetGuideColor() {
		return guide_color_;
	}
	void CharacterTile::SetGuideColor(sf::Color color) {
		guide_color_ = color;
	}

	sf::Color CharacterTile::GetTraceColor() {
		return trace_color_;
	}
	void CharacterTile::SetTraceColor(sf::Color color) {
		trace_color_ = color;
	}

	sf::Color CharacterTile::GetAnimateColor() {
		return animate_color_;
	}
	void CharacterTile::SetAnimateColor(sf::Color color) {
		animate_color_ = color;
	}

	sf::Color CharacterTile::GetStrokeColor() {
		return stroke_color_;
	}
	void CharacterTile::SetStrokeColor(sf::Color color) {
		stroke_color_ = color;
	}

	void CharacterTile::Register() {
		Game::GetInstance()->GetEventManager()->RegisterListener(Event::E_MOUSE_EXITED, this, this);
		Game::GetInstance()->GetEventManager()->RegisterListener(Event::E_MOUSE_MOVED, this, this);
		Game::GetInstance()->GetEventManager()->RegisterListener(Event::E_MOUSE_PRESSED, this, this);
		Game::GetInstance()->GetEventManager()->RegisterListener(Event::E_MOUSE_RELEASED, this, this);
	}

	void CharacterTile::Unregister() {
		Game::GetInstance()->GetEventManager()->UnregisterListener(Event::E_MOUSE_EXITED, this, this);
		Game::GetInstance()->GetEventManager()->UnregisterListener(Event::E_MOUSE_MOVED, this, this);
		Game::GetInstance()->GetEventManager()->UnregisterListener(Event::E_MOUSE_PRESSED, this, this);
		Game::GetInstance()->GetEventManager()->UnregisterListener(Event::E_MOUSE_RELEASED, this, this);
	}

	void CharacterTile::Update(float elapsed_seconds) {
		if (IsAnimating()) {
			// We wait a short while after finishing each stroke
			if (seconds_to_wait_ > 0) {
				seconds_to_wait_ -= elapsed_seconds;
				if (seconds_to_wait_ <= 0) {
					// Reset animation stroke
					current_line_distance_ = 0;
					animating_lines_.clear();
				}
			} else {
				current_line_distance_ += elapsed_seconds * animation_speed_;

				if (animating_lines_.size() > 0) {
					animating_lines_.pop_back();
				}
				sf::RectangleShape rect;
				rect.setFillColor(animate_color_);
				sf::Vector2f p2((float)trace_character_->x(animating_stroke_, animating_lines_.size()),
					(float)trace_character_->y(animating_stroke_, animating_lines_.size()));
				while (true) {
					if (animating_lines_.size() + 1 >= trace_character_->stroke_size(animating_stroke_)) {
						// Finished animating the stroke
						seconds_to_wait_ = wait_seconds_;
						break;
					}

					sf::Vector2f p1 = p2;
					p2 = sf::Vector2f((float)trace_character_->x(animating_stroke_, animating_lines_.size() + 1),
						(float)trace_character_->y(animating_stroke_, animating_lines_.size() + 1));
					sf::Vector2f diff = p2 - p1;
					float distance = sfm::Length(diff);
					if (current_line_distance_ < distance) {
						float ratio = current_line_distance_ / distance;
						sf::Vector2f p = p1 + ratio * diff;
						sf::SetToLine(rect, p1, p, stroke_thickness_);
						animating_lines_.push_back(rect);
						break;
					} else {
						current_line_distance_ -= distance;
						sf::SetToLine(rect, p1, p2, stroke_thickness_);
						animating_lines_.push_back(rect);
					}
				}
			}
		}
	}

	void CharacterTile::Draw(sf::RenderTarget* target) {
		sf::View view = target->getView();
		sf::Vector2u size = target->getSize();
		sf::Vector2f center = sf::Vector2f((float)size.x/2, (float)size.y/2);
		sf::Vector2f my_pos = GetPosition();
		target->setView(sf::View(center - GetPosition(), 2.f * center));

		float width = (float)character_->width();
		float height = (float)character_->height();

		// Draw background
		sf::RectangleShape rect(sf::Vector2f(width, height));
		rect.setFillColor(sf::Color::White);
		target->draw(rect);

		// Draw border lines
		rect.setFillColor(border_color_);
		rect.setSize(sf::Vector2f(width, stroke_thickness_));
		rect.setPosition(0, 0);

		target->draw(rect);
		rect.setPosition(0, height - stroke_thickness_);
		target->draw(rect);
		rect.setSize(sf::Vector2f(stroke_thickness_, height));
		rect.setPosition(0, 0);
		target->draw(rect);
		rect.setPosition(width - stroke_thickness_, 0);
		target->draw(rect);

		// Draw guide lines
		rect.setFillColor(guide_color_);
		rect.setPosition(stroke_thickness_, (height-stroke_thickness_)/2);
		rect.setSize(sf::Vector2f(width - 2*stroke_thickness_, stroke_thickness_));
		target->draw(rect);
		rect.setPosition((width-stroke_thickness_)/2, stroke_thickness_);
		rect.setSize(sf::Vector2f(stroke_thickness_, height - 2*stroke_thickness_));
		target->draw(rect);

		sf::CircleShape circle(0.5f * stroke_thickness_);
		circle.setOrigin(0.5f * stroke_thickness_, 0.5f * stroke_thickness_);
		if (trace_character_ != NULL) {
			// Draw trace character up to the currently animated stroke, or all strokes if no animation
			size_t maxStroke = (animating_stroke_ < 0) ? trace_lines_.size() : std::min((int)trace_lines_.size(), animating_stroke_ + 1);		
			circle.setFillColor(trace_color_);
			for (size_t i = 0; i < maxStroke; i++) {
				circle.setPosition((float)trace_character_->x(i, 0), (float)trace_character_->y(i, 0));
				target->draw(circle);
				for (size_t j = 0; j < trace_lines_[i].size(); j++) {
					target->draw(trace_lines_[i][j]);
					circle.setPosition((float)trace_character_->x(i, j+1), (float)trace_character_->y(i, j+1));
					target->draw(circle);
				}
			}

			// Draw animating stroke
			circle.setFillColor(animate_color_);
			if (animating_lines_.size() > 0) {
				for (size_t j = 0; j < animating_lines_.size(); j++) {
					target->draw(animating_lines_[j]);
					circle.setPosition((float)trace_character_->x(animating_stroke_, j), (float)trace_character_->y(animating_stroke_, j));
					target->draw(circle);
				}
			}
		}

		// Draw current character
		circle.setFillColor(stroke_color_);
		for (size_t i = 0; i < stroke_lines_.size(); i++) {
			circle.setPosition((float)character_->x(i, 0), (float)character_->y(i, 0));
			target->draw(circle);
			for (size_t j = 0; j < stroke_lines_[i].size(); j++) {
				target->draw(stroke_lines_[i][j]);
				circle.setPosition((float)character_->x(i, j+1), (float)character_->y(i, j+1));
				target->draw(circle);
			}
		}
	
		target->setView(view);
	}

	void CharacterTile::ProcessEvent(Event *event) {
		switch(event->type) {
		case Event::E_MOUSE_PRESSED:
			{
				MouseEvent *mouse_event = (MouseEvent*)event;
				switch (mouse_event->button) {
				case Mouse::Left:
					if (is_writing_) {
						break;
					}
					std::cout << "start\n";
					is_writing_ = true;

					last_mouse_ = sf::Vector2f(mouse_event->pos);
					AddStrokePoint(last_mouse_);
					std::cout << "Start Stroke (" << std::dec << last_mouse_.x << "," << last_mouse_.y << ")" << std::endl;
					break;
				case Mouse::Right:
					is_writing_ = false;
					UndoStroke();
					break;
				}
				break;
			}
			
		case Event::E_MOUSE_MOVED:
			if (is_writing_) {
				MouseEvent *mouse_event = (MouseEvent*)event;
				sf::Vector2f mouse_pos = sf::Vector2f(mouse_event->pos);
				if (sf::squaredDistance(mouse_pos, last_mouse_) < MIN_STROKE_DISPLACEMENT_SQUARED) {
					break;
				}

				std::cout << "stroke\n";
				last_mouse_ = mouse_pos;	
				AddStrokePoint(last_mouse_);
				//std::cout << "Add stroke segment " << character_->stroke_size(stroke) << std::endl;
			}
			break;

		case Event::E_MOUSE_RELEASED:
		case Event::E_MOUSE_EXITED:
			if (is_writing_) {
				std::cout << "release\n";
				is_writing_ = false;
				EndStroke();
				SetAnimationStroke(NumStrokes());
				std::cout << "End Stroke" << std::endl;
			}
			break;
		}
	}

	void CharacterTile::Reclassify () {
		if (current_stroke_ == 0) {
			unicode_ = sf::String();
			return;
		}

		zinnia::Result *result = _recognizer->classify(*character_, 1);
		if (result) {
			std::cout << result->size() << std::endl;
			unicode_ = UTF8ToUTF32(result->value(0));
		} else {
			throw "ERROR: " + std::string(_recognizer->what());
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
				sf::SetToLine(rect, p1, p2, stroke_thickness_);
				stroke.push_back(rect);
			}
			lines.push_back(stroke);
		}
	}

	void CharacterTile::CalcStrokeError() {
		size_t stroke = character_->strokes_size() - 1;
		size_t points = character_->stroke_size(stroke);
		size_t trace_points = trace_character_->stroke_size(stroke);
		if (stroke >= trace_character_->strokes_size()) {
			std::cout << "Not a stroke in trace character." << std::endl;
			stroke_errors_.push_back(0);
			return;
		}
		if (points < 2 || trace_points < 2) {
			std::cout << "Not enough points to calculate stroke error." << std::endl;
			stroke_errors_.push_back(0);
			return;
		}

		// We calculate the error by finding the area of the polygon that is between the
		// stroke lines and trace lines

		// The signed area of a planar non-self-intersecting polygon is
		// 1/2 the sum of the determinants of all neighboring vertices pairs
		// A = 1/2 (|x1 x2| + |x2 x3| + ... + |xn x1|)
		//          |y1 y2|   |y2 y3|         |yn y1|
		// counterclockwise order of verices results in positive area

		float area = 0;
		float length = 0;
		sf::Vector2f p1;
		sf::Vector2f p2 = sf::Vector2f((float)trace_character_->x(stroke, 0), (float)trace_character_->y(stroke, 0));
		for (size_t i = 1; i < trace_points; i++) {
			p1 = p2;
			p2 = sf::Vector2f((float)trace_character_->x(stroke, i), (float)trace_character_->y(stroke, i));
			area += p1.x * p2.y - p2.x * p1.y;
			length += sfm::Length(p1 - p2);
		}

		for (int i = points-1; i >= 0; i--) {
			p1 = p2;
			p2 = sf::Vector2f((float)character_->x(stroke, i), (float)character_->y(stroke, i));
			area += p1.x * p2.y - p2.x * p1.y;
		}

		// Case for when stroke lines wraps back to start of trace lines
		p1 = p2;
		p2 = sf::Vector2f((float)trace_character_->x(stroke, 0), (float)trace_character_->y(stroke, 0));
		area += p1.x * p2.y - p2.x * p1.y;

		// We average the error by dividing by the total length of the strokes in the trace character
		float error = abs(area) / length;
		// We also add in the error for the difference in position of the start and end points
		error += sfm::Length(p1 - p2);
		p1 = sf::Vector2f((float)character_->x(stroke, points-1), (float)character_->y(stroke, points-1));
		p2 = sf::Vector2f((float)trace_character_->x(stroke, trace_points-1), (float)trace_character_->y(stroke, trace_points-1));
		error += sfm::Length(p1 - p2);

		stroke_errors_.push_back(error);
	}

}