#pragma once

#include <entt/entt.hpp>
#include "game_data.h"

class BaseSystem{
public:
    virtual void update(float dt);
protected:
    BaseSystem(GameDataRef data) : _data(data){on_init();};
    GameDataRef _data;
    virtual void on_init();
};