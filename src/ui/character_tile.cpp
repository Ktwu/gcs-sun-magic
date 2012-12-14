#include "stdafx.h"
#include "character_tile.h"

#include <iterator>
#include "time.h"

#include "game.h"
#include "assets/gameasset_manager.h"
#include "events/event_manager.h"
#include "tools/sfm.h"
#include "tools/tools.h"
#include "ui/character_tile_style.h"

namespace sun_magic {
	
	static const float MIN_CLASSIFY_SCORE = 0.8f;
	static const float MAX_ERROR = 40.f;

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

	CharacterTile::CharacterTile(float x, float y, float width, float height) :
		UiElement(x, y, width, height),
		writable_(true),
		character_(zinnia::Character::create()),
		unicode_(),
		current_stroke_(0),
		animate_continuously_(false),
		trace_character_(NULL),
		trace_unicode_(),
		animating_stroke_(-1),
		seconds_to_wait_(0),
		current_line_distance_(0),
		is_writing_(false)
	{
		character_->clear();
		character_->set_width((size_t)width);
		character_->set_height((size_t)height);
	
		stroke_lines_.push_back(std::vector<sf::RectangleShape>());

		tilestyle_.SetAnimationSpeed(150.0f)->SetAnimationWait(0.2f);
		tilestyle_.SetStrokeThickness(4)->SetStrokeColor(sf::Color::Black)->SetAnimateColor(sf::Color(150, 150, 150));
		tilestyle_.SetTraceColor(sf::Color(210, 210, 210));
	}

	CharacterTile::~CharacterTile() {
		delete character_;
	}

	CharacterTileStyle* CharacterTile::GetTileStyle() {
		return &tilestyle_;
	}

	
	bool CharacterTile::IsWritable() {
		return writable_;
	}
	void CharacterTile::SetWritable(bool writable) {
		writable_ = writable;
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
		character_->add(current_stroke_, (int)p.x, (int)p.y);
		size_t points = character_->stroke_size(current_stroke_);
		if (points > 1) {
			sf::RectangleShape rect;
			sf::Vector2f prev((float)character_->x(current_stroke_, points-2),
				(float)character_->y(current_stroke_, points-2));
			sfm::SetToLine(rect, prev, p, tilestyle_.stroke_thickness);
			rect.setFillColor(tilestyle_.stroke_color);
			stroke_lines_.back().push_back(rect);
		}
	}
	// Consider every two consecutive lines. If the angle between the lines is small enough remove the mid point.
	size_t SmoothPoints(std::vector<sf::Vector2f>& points1, std::vector<sf::Vector2f>& points2, size_t index) {
		static const float THRESHOLD_ANGLE = cos(sfm::DEGTORAD(11));

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
		CreateLines(stroke_lines_, character_, tilestyle_.stroke_color, current_stroke_, current_stroke_+1);
		stroke_lines_.push_back(std::vector<sf::RectangleShape>());

		current_stroke_++;
		CalcError();
		Reclassify();

		if (trace_character_ != NULL && GetError() <= MAX_ERROR) {
			SetAnimationStroke(NumStrokes());
		}

	}
	void CharacterTile::UndoStroke() {
		if (current_stroke_ < 0)
			return;
	
		// There is no remove stroke, so we just create a copy up to the last stroke.
		size_t strokes = character_->strokes_size();	
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
			if (trace_character_ != NULL) {
				stroke_errors_.pop_back();
				if (GetError() <= MAX_ERROR)
					SetAnimationStroke(NumStrokes());
			}
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
	float CharacterTile::GetError() {
		float error = 0;
		for (int i = 0; i < stroke_errors_.size(); i++)
			error = std::max(error, stroke_errors_[i]);
		return error;
	}

	zinnia::Character * CharacterTile::GetTraceCharacter() {
		return trace_character_;
	}
	sf::String CharacterTile::GetTraceUnicode() {
		return trace_unicode_;
	}
	void CharacterTile::SetTraceCharacter(zinnia::Character *character) {
		trace_character_ = tools::Resize(character, (size_t)rect_.width, (size_t)rect_.height);
		
		trace_unicode_ = (character == NULL) ? "" : tools::UTF8ToUTF32(character->value()); 
		animating_stroke_ = -1;
	
		trace_lines_.clear();
		if (trace_character_ != NULL) {
			CreateLines(trace_lines_, trace_character_, tilestyle_.trace_color, 0, trace_character_->strokes_size());
		}
	}

	bool CharacterTile::IsAnimating() {
		return animating_stroke_ >= 0;
	}
	bool CharacterTile::IsAnimatingContinuously() {
		return animate_continuously_;
	}
	void CharacterTile::SetAnimatingContinuously(bool animate_continuously) {
		animate_continuously_ = animate_continuously;
		if (animate_continuously_ && animating_stroke_ == -1) {
			animating_stroke_ = 0;
		}
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
					if (animate_continuously_) {
						animating_stroke_ = (animating_stroke_ + 1) % trace_character_->strokes_size();
					}
				}
			} else {
				current_line_distance_ += elapsed_seconds * tilestyle_.animation_speed;

				if (animating_lines_.size() > 0) {
					animating_lines_.pop_back();
				}
				sf::RectangleShape rect;
				rect.setFillColor(tilestyle_.animate_color);
				sf::Vector2f p2((float)trace_character_->x(animating_stroke_, animating_lines_.size()),
					(float)trace_character_->y(animating_stroke_, animating_lines_.size()));
				while (true) {
					if (animating_lines_.size() + 1 >= trace_character_->stroke_size(animating_stroke_)) {
						// Finished animating the stroke
						seconds_to_wait_ = tilestyle_.wait_seconds;
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
						sfm::SetToLine(rect, p1, p, tilestyle_.stroke_thickness);
						animating_lines_.push_back(rect);
						break;
					} else {
						current_line_distance_ -= distance;
						sfm::SetToLine(rect, p1, p2, tilestyle_.stroke_thickness);
						animating_lines_.push_back(rect);
					}
				}
			}
		}
	}

	void CharacterTile::Draw(sf::RenderTarget* target) {
		sf::Vector2f size(rect_.width, rect_.height);

		// Draw background and border
		UiElement::Draw(target);

		// Draw guide lines
		sf::RectangleShape rect(size);
		rect.setOutlineThickness(0);
		rect.setFillColor(tilestyle_.guide_color);
		rect.setPosition(0, (size.y - tilestyle_.stroke_thickness)/2);
		rect.setSize(sf::Vector2f(size.x, tilestyle_.stroke_thickness));
		target->draw(rect);
		rect.setPosition((size.x - tilestyle_.stroke_thickness)/2, 0);
		rect.setSize(sf::Vector2f(tilestyle_.stroke_thickness, size.y));
		target->draw(rect);

		// Draw error
		if (stroke_errors_.size() > 0) {
			sf::Text error_text;
			float error = GetError();
			if (error > MAX_ERROR)
				error_text.setColor(tilestyle_.error_bad_color);
			else
				error_text.setColor(tilestyle_.error_good_color);
			error_text.setCharacterSize(20);
			char buf[128];
			std::sprintf(buf, "%.0f", error);
			error_text.setString("Error: " + sf::String(buf));
			sf::FloatRect error_bounds = error_text.getLocalBounds();
			error_text.setPosition(0, size.y - error_bounds.height - 5);
			target->draw(error_text);
		}

		sf::CircleShape circle(0.5f * tilestyle_.stroke_thickness);
		circle.setOrigin(0.5f * tilestyle_.stroke_thickness, 0.5f * tilestyle_.stroke_thickness);
		if (trace_character_ != NULL) {
			// Draw trace character up to the currently animated stroke	
			circle.setFillColor(tilestyle_.animate_color);
			for (int i = 0; i < animating_stroke_; i++) {
				circle.setPosition((float)trace_character_->x(i, 0), (float)trace_character_->y(i, 0));
				target->draw(circle);
				for (size_t j = 0; j < trace_lines_[i].size(); j++) {
					trace_lines_[i][j].setFillColor(tilestyle_.animate_color);
					target->draw(trace_lines_[i][j]);
					circle.setPosition((float)trace_character_->x(i, j+1), (float)trace_character_->y(i, j+1));
					target->draw(circle);
				}
			}

			// Draw strokes after animating stroke
			circle.setFillColor(tilestyle_.trace_color);
			for (size_t i = std::max(0, animating_stroke_); i < trace_lines_.size(); i++) {
				circle.setPosition((float)trace_character_->x(i, 0), (float)trace_character_->y(i, 0));
				target->draw(circle);
				for (size_t j = 0; j < trace_lines_[i].size(); j++) {
					trace_lines_[i][j].setFillColor(tilestyle_.trace_color);
					target->draw(trace_lines_[i][j]);
					circle.setPosition((float)trace_character_->x(i, j+1), (float)trace_character_->y(i, j+1));
					target->draw(circle);
				}
			}

			// Draw animating stroke
			circle.setFillColor(tilestyle_.animate_color);
			if (animating_lines_.size() > 0) {
				for (size_t j = 0; j < animating_lines_.size(); j++) {
					target->draw(animating_lines_[j]);
					circle.setPosition((float)trace_character_->x(animating_stroke_, j), (float)trace_character_->y(animating_stroke_, j));
					target->draw(circle);
				}
			}
		}

		// Draw current character
		circle.setFillColor(tilestyle_.stroke_color);
		for (size_t i = 0; i < stroke_lines_.size(); i++) {
			size_t stroke_size = stroke_lines_[i].size();
			if (stroke_size == 0)
				continue;
			circle.setPosition((float)character_->x(i, 0), (float)character_->y(i, 0));
			target->draw(circle);
			for (size_t j = 0; j < stroke_size; j++) {
				target->draw(stroke_lines_[i][j]);
				circle.setPosition((float)character_->x(i, j+1), (float)character_->y(i, j+1));
				target->draw(circle);
			}
		}
	}

	void CharacterTile::ProcessEvent(Event event) {
		if (!writable_)
			return;

		switch(event.type) {
		case Event::E_MOUSE_PRESSED:
			if (!is_writing_ && event.mouseButton.button == Mouse::Left) {
				is_writing_ = true;

				last_mouse_ = sf::Vector2f((float)event.relativeMousePosition.x, (float)event.relativeMousePosition.y);
				AddStrokePoint(last_mouse_);
				std::cout << "Start Stroke (" << std::dec << last_mouse_.x << "," << last_mouse_.y << ")" << std::endl;
			}
			break;
			
		case Event::E_MOUSE_MOVED:
			if (is_writing_) {
				sf::Vector2f mouse_pos = sf::Vector2f((float)event.relativeMousePosition.x, (float)event.relativeMousePosition.y);
				if (sfm::squaredDistance(mouse_pos, last_mouse_) < MIN_STROKE_DISPLACEMENT_SQUARED) {
					break;
				}

				last_mouse_ = mouse_pos;	
				AddStrokePoint(last_mouse_);
			}
			break;

		case Event::E_MOUSE_RELEASED:
			if (event.mouseButton.button == Mouse::Right) {
				is_writing_ = false;
				UndoStroke();
				break;
			}
		case Event::E_MOUSE_EXITED:
			if (is_writing_) {
				is_writing_ = false;
				EndStroke();
				std::cout << "End Stroke" << std::endl;
			}
			break;
		}
	}

	void CharacterTile::Reclassify () {
		EventManager* manager = Game::GetInstance()->GetEventManager();
		Event event;

		event.type = Event::E_HIRAGANA_DRAWN;
		event.focus = this;

		if (current_stroke_ == 0) {
			unicode_ = sf::String();
			manager->AddEvent(event);
			return;
		}

		zinnia::Result *result = _recognizer->classify(*character_, 1);
		if (result) {
			if (result->score(0) > MIN_CLASSIFY_SCORE && GetError() <= MAX_ERROR) {
				unicode_ = tools::UTF8ToUTF32(result->value(0));
			} else {
				unicode_ = sf::String();
			}
		} else {
			throw "ERROR: " + std::string(_recognizer->what());
		}
		delete result;

		event.message = unicode_;
		manager->AddEvent(event);
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
				sfm::SetToLine(rect, p1, p2, tilestyle_.stroke_thickness);
				stroke.push_back(rect);
			}
			lines.push_back(stroke);
		}
	}

	void CharacterTile::CalcError() {
		if (trace_character_ == NULL)
			return;

		size_t stroke = character_->strokes_size() - 1;
		if (stroke >= trace_character_->strokes_size())
			return;

		size_t points = character_->stroke_size(stroke);
		size_t trace_points = trace_character_->stroke_size(stroke);

		if (points < 2 || trace_points < 2) {
			// Not enough points to calculate
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