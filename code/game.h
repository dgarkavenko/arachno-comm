#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>
#include "fps_counter.h"
#include "observer_system.h"


#include "texture_loader_system.h"
#include "font_system.h"
#include "debug_system.h"

#include "hand_system.h"
#include "tween_system.h"

#include "game_data.h"


class Game{
    
    GameDataRef _data;
    FPSCounter *fps;

public:
    Game(int w, int h, std::string title);
    void loop();
    void update_systems(float dt);

};