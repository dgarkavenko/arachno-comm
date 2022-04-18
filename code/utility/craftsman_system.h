#pragma once

#include "game_data.h"
#include "components.h"
#include <map>

class ConstructorSystem{
public: 
    ConstructorSystem(GameDataRef data) : _data(data){init();};
    void update(float dt);
  
protected:
    GameDataRef _data;
    void init();

    std::map<std::string, sf::Font*> fonts; 
    std::map<std::string, sf::Texture*> textures; 

    sf::Font* get_font(std::string path);
    sf::Texture* get_texture(std::string name);

    void text_construct(entt::registry &registry, entt::entity entity);
    void sprite_construct(entt::registry &registry, entt::entity entity);
};