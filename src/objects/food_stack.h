#pragma once

#include "stdafx.h"

namespace sun_magic {

  class FoodStack
  {
  public:
    FoodStack(int animons, float height, float width);
    ~FoodStack();

    void draw(sf::RenderTarget *target);
    void grow(int animon);
    void shrink(int animon);

  protected:
    int num_animons_;
    int *stack_level_;  //each index is an animon
    float animon_dist_; //distance between each animon

    float food_size_height_;
    float food_size_width_;
    int max_food_;

    float bowl_size_height_;
    float bowl_size_width_;
    float bowl_position_height_; //bowl distance from floor
  };

}

