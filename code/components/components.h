#pragma once
#include <string>
#include <functional>

#include <SFML/Graphics.hpp>
#include "math_functions.h"


namespace blanks{

    struct Text
    {
        std::string text{"blank"};
        std::string font{"PT_Sans/PTSans-Regular.ttf"};
        int size = 18;
        sf::Color color{sf::Color::Blue};
        sf::Text **bind_ptr {nullptr};
    };

    struct Sprite{
        std::string texture_path;
        sf::Vector2f scale {1.0f, 1.0f};
        sf::Vector2f origin {.5f, .5f};
        std::function<void(sf::Sprite&)> on_create;
    };
}

namespace tag{
    struct InHand {};
    struct Hover {};
    struct Drag {};
    struct Selectable {};
}

struct Inertia{
    sf::Vector2f velocity;
    sf::Vector2f acceleration;
    sf::Vector2f previous_position;
    float drag = 0.048f;
};

struct TweenComponent{
    sf::Vector2f target {0,0};
    sf::Angle rotation;
    float duration = 1;
    float time = 0;
    float delay = 0;
    TweenFunction tween_function;
};