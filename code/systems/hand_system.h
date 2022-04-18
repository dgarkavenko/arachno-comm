#pragma once
#include "game_data.h"
#include "components.h"

class HandSystem
{
public:
    HandSystem(GameDataRef data) : _data(data){init();};
    void update(float dt);
    entt::observer observer;

protected:
    GameDataRef _data;
    void init();
    void set_in_hand_positions();
};
