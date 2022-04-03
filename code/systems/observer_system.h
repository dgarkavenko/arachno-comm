#pragma once
#include "base_system.h"
#include "entt/entt.hpp"

#pragma once
#include "base_system.h"

#pragma once
#include "base_system.h"

class ObserverSystem : public BaseSystem{
public:
    ObserverSystem(GameDataRef data) : BaseSystem::BaseSystem(data){};
    entt::observer observer;
    void update(float dt) override;
protected:
    void on_init() override;

};

struct PickleComponent{
    int whinner;
    bool flagged;
};