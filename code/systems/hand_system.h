#pragma once
#include "base_system.h"

class HandSystem : public BaseSystem{
public:
    HandSystem(GameDataRef data) : BaseSystem::BaseSystem(data){on_init();};
    void update(float dt) override;
    entt::observer observer;

protected:
    void on_init() override;
    void set_in_hand_positions();
};