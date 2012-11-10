#include "stdafx.h"
#include "dictionary.h"
#include "game.h"

namespace sun_magic{

  Dictionary::Dictionary(float x, float y, float width, float height) : 
      GameObject(x, y, width, height) { }
  Dictionary::~Dictionary() {}

  void Dictionary::Update(float elapsed_time){}

  void Dictionary::Draw(sf::RenderTarget *target){
		sf::Vector2u size = target->getSize();
    float window_width = (float)size.x;
    float window_height = (float)size.y;
	  float width = window_width * (.25f);
	  float height = window_height * (.5f);
    sf::Vector2f position = sf::Vector2f(window_width - width, window_height - height);

	  sf::RectangleShape rect(sf::Vector2f(width, height));
    rect.setPosition(position);
	  rect.setFillColor(sf::Color::White);

    
	  target->draw(rect);
    
  }

  void Dictionary::DictWords(sf::RenderTarget *target, std::vector<std::string> words){
    sf::Vector2u size = target->getSize();
    float window_width = (float)size.x;
    float window_height = (float)size.y;
    float edge_space_width = 10;
    float edge_space_height = 10;
	  float width = window_width * (.25f);
	  float height = window_height * (.5f);
    float start_width = window_width - width + edge_space_width;
    float start_height = window_height - height + edge_space_height;
    float word_width = 0;
    float word_height = -(height/10);
    
    for(int i = 0; i < (int)words.size(); i++){
      sf::Text text(words[i]);
      text.setCharacterSize(15);
      text.setColor(sf::Color::Black);
      if(i % 2 == 0){
        word_width = 0;
        word_height += height/10;
      }
      else{
        word_width = width/2;
      }
      text.setPosition(sf::Vector2f(start_width + word_width, start_height + word_height));
      target->draw(text);
    }
  }

  void Dictionary::Register(){}
  void Dictionary::Unregister(){}
}











