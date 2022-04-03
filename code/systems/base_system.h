#pragma once

#include <entt/entt.hpp>
#include "game_data.h"


class BaseSystem{
public:
    virtual void update(float dt);
    void init(GameDataRef ref);
protected:
    GameDataRef _data;
    virtual void on_init();
};