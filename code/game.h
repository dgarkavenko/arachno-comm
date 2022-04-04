#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>
#include "draw_system.h"
#include "fps_counter.h"
#include "observer_system.h"

#include "game_data.h"

class Game{
    
    sf::Clock _clock;
    GameDataRef _data;
    DrawSystem *draw;
    FPSCounter *fps;
    ObserverSystem *test_observer;
    //Board board;

public:
    Game(int w, int h, std::string title);
    void run();
    void update_systems(float dt);

};