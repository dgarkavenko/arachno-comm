#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>
#include "draw_system.h"
#include "fps_counter.h"
#include "game_data.h"

class Game{
    GameDataRef _data;
    sf::Clock _clock;
    DrawSystem _draw;
    FPSCounter _fps;

public:
    Game(int w, int h, std::string title);
    void run();
    void update_systems(float dt);

};