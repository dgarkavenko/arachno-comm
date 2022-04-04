#include "fps_counter.h"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Main.hpp>
#include <fmt/core.h>


void fps_counter_consturct(entt::registry &registry, entt::entity entity)
{
    sf::Font* font = new sf::Font;

    bool loaded = font->loadFromFile("../assets/PT_Sans/PTSans-Regular.ttf");
    if(!loaded)
        fmt::print("Font not found");

    sf::Text &text = registry.emplace<sf::Text>(entity);
    text.setFont(*font);
    text.setCharacterSize(18);
    text.setFillColor(sf::Color::Red);
    registry.on_construct<FPSCounterComponent>().disconnect<&fps_counter_consturct>();
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

void FPSCounter::on_init()
{
    _data->registry.on_construct<FPSCounterComponent>().connect<&fps_counter_consturct>();
    _data->registry.emplace<FPSCounterComponent>(_data->registry.create());
}