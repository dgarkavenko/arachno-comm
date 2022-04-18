#pragma once
#include "game_data.h"
#include "components.h"

class FPSCounter
{
public:
    FPSCounter(GameDataRef data) : _data(data){init();};
    void update(float dt);
protected:
    GameDataRef _data;
    void init();
};

struct FPSCounterComponent{
    static const unsigned measure_size = 16;
    float measures[measure_size];
    unsigned current_index = 0;
};
