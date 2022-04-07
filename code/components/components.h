#pragma once
#include <string>
#include <SFML/Graphics.hpp>

struct InHand {};

struct TweenComponent{
    sf::Vector2f target {0,0};
    sf::Angle rotation;
    float duration = 1;
    float time = 0;
    float delay = 0;
};