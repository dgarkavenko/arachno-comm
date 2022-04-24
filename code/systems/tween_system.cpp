#include "tween_system.h"

#include <fmt/core.h>
#include <components.h>
#include <algorithm>
#include <math.h>
#include "math_functions.h"

using namespace sf;


void TweenSystem::init()
{
    update_delegate delegate{};
    delegate.connect<&TweenSystem::update>(this);
    _data->delegates.push_back(delegate);
}

void TweenSystem::update(float dt)
{
    tween(dt);
    inertia(dt);
}

void TweenSystem::inertia(float dt)
{
    auto dragged = _data->registry.view<Inertia, tag::Drag, sf::Sprite>();
    for (auto entity : dragged){
        auto &inertia = dragged.get<Inertia>(entity);
        auto position = dragged.get<sf::Sprite>(entity).getPosition();
        auto travel = (position - inertia.previous_position) / std::max(0.01f, dt);
        inertia.velocity = smoothDamp(inertia.velocity, travel, inertia.acceleration, .05f, 9000, dt);
        inertia.previous_position = position;
    }

    auto view = _data->registry.view<Inertia, Sprite>(entt::exclude<tag::Drag>);
    for (auto entity : view)
    {
        auto &inertia = view.get<Inertia>(entity);

        if (inertia.velocity.lengthSq() < .1f)
        {
            _data->registry.erase<Inertia>(entity);
            continue;
        }

        auto travel = inertia.velocity * dt;
        Sprite &sprite = view.get<Sprite>(entity);
        sprite.setPosition(sprite.getPosition() + travel);
        inertia.velocity *= (1-inertia.drag);
    }
}

void TweenSystem::tween(float dt)
{
    auto view = _data->registry.view<TweenComponent, Sprite>();
    for (auto entity : view)
    {
        auto &tween = view.get<TweenComponent>(entity);
        Sprite &sprite = view.get<Sprite>(entity);

        tween.time += dt;

        float t = tween.time / tween.duration;

        sprite.setPosition(tween.tween_function(sprite.getPosition(), tween.target, t));

        auto delta = (tween.rotation - sprite.getRotation()).wrapSigned();
        sprite.rotate(delta * tween.rotation_function(0.0f, 1.0f, t));

        if (tween.time > tween.duration)
            _data->registry.erase<TweenComponent>(entity);
    }
}
