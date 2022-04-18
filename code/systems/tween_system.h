#pragma once
#include "game_data.h"
#include "components.h"

class TweenSystem
{
public:
    TweenSystem(GameDataRef data) : _data(data){init();};
    void update(float dt);
protected:
    GameDataRef _data;
    void init();
    void tween(float dt);
    void inertia(float dt);
};
