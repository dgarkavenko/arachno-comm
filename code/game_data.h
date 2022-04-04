#pragma once
#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>

#define RENDER_WIDTH 800
#define RENDER_HEIGHT 600

struct GameData{
    sf::RenderWindow window;
    entt::registry registry;
};

typedef std::shared_ptr<GameData> GameDataRef;
