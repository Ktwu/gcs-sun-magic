#include "stdafx.h"
#include "progressbar.h"

#include "game.h"
#include "assets/gameasset_manager.h"
#include "events/event_manager.h"

namespace sun_magic {

	ProgressBar::ProgressBar(float x, float y, float height, std::string base_name, std::string increment_name) :
		GameObject(x, y, 0, height),
		base_(),
		increment_(),
		progress_()
	{
		GameAssetManager* manager = GameAssetManager::GetInstance();
		SetBaseSprite(sf::Sprite(*manager->GetTexture(this, base_name)));
		SetIncrementSprite(sf::Sprite(*manager->GetTexture(this, increment_name)));
	}

	ProgressBar::~ProgressBar()	{
		 GameAssetManager::GetInstance()->ReturnTextures(this);
	}

	void ProgressBar::SetBaseSprite(sf::Sprite sprite) {
		base_ = sprite;
		this->rect_.width = std::max((float)base_.getLocalBounds().width, (float)increment_.getLocalBounds().width);
	}
	sf::Sprite ProgressBar::GetBaseSprite() {
		return base_;
	}

	void ProgressBar::SetIncrementSprite(sf::Sprite sprite) {
		increment_ = sprite;
		this->rect_.width = std::max((float)base_.getLocalBounds().width, (float)increment_.getLocalBounds().width);
	}
	sf::Sprite ProgressBar::GetIncrementSprite() {
		return increment_;
	}

	float ProgressBar::GetProgress() {
		return progress_;
	}
	void ProgressBar::SetProgress(float progress) {
		progress_ = std::max(0.f, std::min(1.f, progress));
	}

	void ProgressBar::Register() {}
	void ProgressBar::Unregister() {}

	void ProgressBar::Update(float elapsed_time) {
	}
	void ProgressBar::Draw(sf::RenderTarget* target) {
		// Draw base sprite
		sf::FloatRect bounds = base_.getLocalBounds();
		base_.setPosition(0.5f * (rect_.width - bounds.width), rect_.height - bounds.height);
		target->draw(base_);

		// Stack increment sprites as needed
		float y = rect_.height - 0.8f * bounds.height;
		int increment_height = increment_.getTexture()->getSize().y;
		int bar_height = (int)((1.f - progress_) * y);
		sf::IntRect rect = increment_.getTextureRect();
		for (; y >= bar_height; y -= 0.9f * increment_height) {
			int height = std::min(increment_height, (int)y - bar_height);
			rect.top = (int)(increment_height - height);
			rect.height = height;
			increment_.setTextureRect(rect);
			increment_.setPosition(0, y - height);
			target->draw(increment_);
		}
	}
}