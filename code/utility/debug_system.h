#pragma once
#include "game_data.h"
#include "blanks.h"


static std::string debug_text(sf::Sprite &sprite){
    return fmt::format("{} : {}", sprite.getPosition().x, sprite.getPosition().y);
}

class Debuger{

public:

    sf::Text *debug_text_ptr;

    Debuger(GameDataRef data) : _data(data)
    {
        auto debug_text = _data->registry.create();
        auto& blank = _data->registry
            .emplace<blanks::Text>(
                debug_text,
                "Hello", 
                "FreeMono/FreeMono.ttf",
                18,
                sf::Color::White,
                &debug_text_ptr
                );
       
    }

    void update(){

        std::string output_string = fmt::format("{:>10} {:<20}{:<20}{:<20}{:<20}",
            "entity",
            "sf:Sprite",
            "sf::Text",
            "blanks::Sprite",
            "blanks::Text");

        if(debug_text_ptr == nullptr)
            return;

        _data->registry.each([&](auto entity) {

            output_string += fmt::format("\n{:>10} {:<20}{:<20}{:<20}{:<20}",
                (unsigned)entity,
                _data->registry.all_of<sf::Sprite>(entity),
                _data->registry.all_of<sf::Text>(entity),
                _data->registry.all_of<blanks::Sprite>(entity),
                _data->registry.all_of<blanks::Text>(entity)

                );
        });

        debug_text_ptr->setString(output_string);
    }

    GameDataRef _data;

};
