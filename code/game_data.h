#pragma once
#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>

struct GameData{
    sf::RenderWindow window;
    entt::registry registry;
};

typedef std::shared_ptr<GameData> GameDataRef;
