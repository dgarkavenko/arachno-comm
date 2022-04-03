#pragma once

#include "base_system.h"

class FPSCounter : public BaseSystem{
public:
    FPSCounter(GameDataRef data) : BaseSystem::BaseSystem(data){};
    void update(float dt) override;
protected:
    void on_init() override;
};

struct FPSCounterComponent{
    static const unsigned measure_size = 16;
    float measures[measure_size];
    unsigned current_index = 0;
};