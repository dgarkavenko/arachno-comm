#pragma once
#include "base_system.h"
#include "entt/entt.hpp"

class ObserverSystem : public BaseSystem{
public:
    void update(float dt) override;
    entt::observer observer;
protected:
    void on_init() override;
};

struct PickleComponent{
    int whinner;
    bool flagged;
};


class Stinky{
public:
    int A;
    Stinky (int a) : A(a){};
};