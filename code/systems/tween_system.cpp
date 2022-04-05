#include "tween_system.h"
#include "card_template.h"
#include <fmt/core.h>


#include <algorithm>
#include <math.h>
using namespace sf;

float smoothstep(float from, float to, float t) {
    t = std::clamp(t, 0.f, 1.f);
    t = (float)(-2.0 * (double)t * (double)t * (double)t + 3 * (double)t * (double)t);
    return (float)((double)to * (double)t + (double)from * (1.0 - (double)t));
}

Vector2f smoothstep(Vector2f from, Vector2f to, float t) {
    // Scale, bias and saturate x to 0..1 range
    return {smoothstep(from.x, to.x, t), smoothstep(from.y, to.y, t),};
}

void TweenSystem::update(float dt) {
    auto view = _data->registry.view<TweenComponent, Sprite>();
    for (auto entity : view) {
        auto& tween = view.get<TweenComponent>(entity);
        Sprite &sprite = view.get<Sprite>(entity);

        tween.time += dt;

        if(tween.time < 0)
            continue;

        float t = tween.time / tween.duration;
        Vector2f smooth = smoothstep(sprite.getPosition(), tween.target, t);
        sprite.setPosition(smooth);

        
        auto delta = (tween.rotation - sprite.getRotation()).wrapSigned();
        sprite.rotate(delta * smoothstep(0.0f,1.0f,t));
        
        Text* text = _data->registry.try_get<Text>(entity);
        if(text){

            text->setPosition(sprite.getPosition());
            text->setFillColor(sf::Color::White);
            text->setString(fmt::format("Tween: {:.1f}", tween.time));
        }


        if(tween.time > tween.duration){
            _data->registry.erase<TweenComponent>(entity);
            continue;
        }
    }
}

void TweenSystem::on_init() {
    _data->registry.on_construct<TweenComponent>().connect<&TweenSystem::on_tween_consturct>(this);
}

void TweenSystem::on_tween_consturct(entt::registry &registry, entt::entity entity) {
    _data->registry.emplace<Text>(entity);
}
