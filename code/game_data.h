#pragma once
#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>
#include <fmt/core.h>

#define RENDER_WIDTH 1024
#define RENDER_HEIGHT 768

struct GameData{
    sf::RenderWindow window;
    entt::registry registry;
};

typedef std::shared_ptr<GameData> GameDataRef;
