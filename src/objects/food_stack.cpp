#pragma once

#include "stdafx.h"
#include "food_stack.h"

#include "game.h"

namespace sun_magic {

  FoodStack::FoodStack(int animons, float height, float width) :
    num_animons_(animons),
    max_food_(5),
    food_size_height_(50.f),
    food_size_width_(100.f),
    bowl_size_height_(50.f),
    bowl_size_width_(120.f),
    bowl_position_height_(500.f),
    animon_dist_(width/num_animons_)  //random positions
  { 
    stack_level_ = new int[num_animons_]; //starts all levels at 0
  }

  FoodStack::~FoodStack() 
  { 
    delete(stack_level_);
  }


  void FoodStack::draw(sf::RenderTarget *target){
    for(int i = 0; i < num_animons_; i++){
      sf::RectangleShape rect(sf::Vector2f(bowl_size_width_, bowl_size_height_));
      rect.setPosition(sf::Vector2f(animon_dist_ * (i+1), bowl_position_height_));

		  rect.setFillColor(sf::Color::Red);
		  rect.setOutlineColor(sf::Color::Black);
		  rect.setOutlineThickness(0.5);
		  target->draw(rect);

      for(int j = 0; j < stack_level_[i]; j++){
        sf::RectangleShape rect(sf::Vector2f(food_size_width_, food_size_height_));
        rect.setPosition(sf::Vector2f(animon_dist_ * (i+1), bowl_position_height_ + food_size_height_ * j));
        rect.setFillColor(sf::Color::White);
        rect.setOutlineColor(sf::Color::Black);
        rect.setOutlineThickness(0.5);
        target->draw(rect);
      }
    }
  }

  void FoodStack::grow(int animon){
    if(stack_level_[animon] < max_food_)
      stack_level_[animon]++;
  }

  void FoodStack::shrink(int animon){
    if(stack_level_[animon] != 0)
      stack_level_[animon]--;
  }
}