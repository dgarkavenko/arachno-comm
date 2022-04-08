#pragma once
#include <string>
#include <SFML/Graphics.hpp>


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
        sf::Vector2f scale;
    };
}

namespace tag{
    struct InHand {};
    struct Hover {};
    struct Drag {};
}

struct TweenComponent{
    sf::Vector2f target {0,0};
    sf::Angle rotation;
    float duration = 1;
    float time = 0;
    float delay = 0;

    int mode = 0;
};