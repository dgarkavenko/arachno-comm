#pragma once

#include "base_system.h"

class DrawSystem : public BaseSystem{
public:
    DrawSystem(GameDataRef data) : BaseSystem::BaseSystem(data){};
    void update(float dt) override;
};