#include "observer_system.h"
#include "fmt/core.h"

unsigned num_frames;
void ObserverSystem::update(float dt)
{
    num_frames++;
    entt::registry &reg = _data->registry;

    auto view = reg.view<PickleComponent>();
    for (auto entity : view)
    {
        auto &pickle = view.get<PickleComponent>(entity);
        if (pickle.whinner == num_frames % 1000)
            reg.patch<PickleComponent>(entity, [](PickleComponent &p){ p.flagged = true; });
    }

    for (auto entity : observer)
        fmt::print("Flagged: {}\n", (u_int32_t)entity);

    observer.clear();
}

void ObserverSystem::on_init()
{
    observer.connect(_data->registry, entt::collector.update<PickleComponent>());
    //observer = entt::observer{_data->registry, entt::collector.update<PickleComponent>()};
}
