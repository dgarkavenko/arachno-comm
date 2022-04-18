#include "fps_counter.h"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Main.hpp>
#include <fmt/core.h>
#include "components.h"


void FPSCounter::init()
{
    update_delegate delegate{};
    delegate.connect<&FPSCounter::update>(this);
    _data->delegates.push_back(delegate);

    entt::entity fps_counter = _data->registry.create();
    _data->registry.emplace<FPSCounterComponent>(fps_counter);
    _data->registry.emplace_or_replace<blanks::Text>(fps_counter, "", "PT_Sans/PTSans-Regular.ttf", 20, sf::Color::Red);
}

void FPSCounter::update(float dt)
{
    auto &reg = _data->registry;
    {
        auto view = reg.view<FPSCounterComponent, sf::Text>();
        for (auto entity : view)
        {
            auto [fps, text] = view.get<FPSCounterComponent, sf::Text>(entity);
            fps.measures[(fps.current_index++) % fps.measure_size] = dt;

            float sum = 0.0f;
            for (unsigned i = 0; i < fps.measure_size; i++)
                sum += fps.measures[i];

            text.setString(fmt::format("fps: {:.1f}", 1.0f / sum * fps.measure_size));
        }
    }
}
