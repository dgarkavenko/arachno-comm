#pragma once
#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>
#include <fmt/core.h>
#include <vector>

#define RENDER_WIDTH 1024
#define RENDER_HEIGHT 768

typedef entt::delegate<void(float)> update_delegate;


struct GameData{
    sf::RenderWindow window;
    entt::registry registry;
    std::vector<update_delegate> delegates;    
};

typedef std::shared_ptr<GameData> GameDataRef;
