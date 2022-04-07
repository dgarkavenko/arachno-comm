#pragma once

#include "game_data.h"
#include "blanks.h"
#include <map>

class FontSystem{
public: 
    FontSystem(GameDataRef data);
    sf::Font* get_font(std::string path);
    void text_construct(entt::registry &registry, entt::entity entity);

    GameDataRef _data;
    std::map<std::string, sf::Font*> fonts; 
};