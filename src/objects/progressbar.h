#pragma once

#include "stdafx.h"
#include "events/event.h"
#include "events/gameevent.h"
#include "objects/game_object.h"

namespace sun_magic {

	class ProgressBar : public GameObject
	{
	public:
		ProgressBar(float x, float y, float height, sf::Sprite base, sf::Sprite increment);
		ProgressBar(float x, float y, float height, sf::String base_name, sf::String increment_name);
		~ProgressBar();

		void SetBaseSprite(sf::Sprite sprite);
		sf::Sprite GetBaseSprite();
		void SetIncrementSprite(sf::Sprite sprite);
		sf::Sprite GetIncrementSprite();

		float GetProgress();
		void SetProgress(float progress);
		
		void Register();
		void Unregister();

		void Update(float elapsed_time);
		void Draw(sf::RenderTarget* target);

	protected:
		sf::Sprite base_;
		sf::Sprite increment_;
		float progress_;
	};
	
}