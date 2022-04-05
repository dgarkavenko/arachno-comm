#pragma once
#include "base_system.h"

class TweenSystem : public BaseSystem{
public:
    TweenSystem(GameDataRef data) : BaseSystem::BaseSystem(data){on_init();};
    void update(float dt) override;
protected:
    void on_init() override;
    void on_tween_consturct(entt::registry &registry, entt::entity entity);

};

