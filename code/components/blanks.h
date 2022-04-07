#pragma once
#include <SFML/Graphics.hpp>
#include <string>

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