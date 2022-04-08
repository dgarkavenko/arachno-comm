#include "tween_system.h"
#include <fmt/core.h>
#include <components.h>


#include <algorithm>
#include <math.h>

using namespace sf;

float lerp(float a, float b, float f)
{
    return a + f * (b - a);
}

float smoothstep(float from, float to, float t) {
    t = std::clamp(t, 0.f, 1.f);
    t = (float)(-2.0 * (double)t * (double)t * (double)t + 3 * (double)t * (double)t);
    return (float)((double)to * (double)t + (double)from * (1.0 - (double)t));
}

float easeOutQuint(float from, float to, float t){
    t = 1 - std::pow(1 - t, 5);
    return lerp(from, to, t);
}

Vector2f easeOutQuint(Vector2f from, Vector2f to, float t) {
    return {easeOutQuint(from.x, to.x, t), easeOutQuint(from.y, to.y, t),};
}

Vector2f smoothstep(Vector2f from, Vector2f to, float t) {
    return {smoothstep(from.x, to.x, t), smoothstep(from.y, to.y, t),};
}

void TweenSystem::update(float dt) {
    auto view = _data->registry.view<TweenComponent, Sprite>();
    for (auto entity : view) {
        auto& tween = view.get<TweenComponent>(entity);
        Sprite &sprite = view.get<Sprite>(entity);

        tween.time += dt;

        float t = tween.time / tween.duration;
        
        Vector2f smooth = tween.mode == 0 ?
            smoothstep(sprite.getPosition(), tween.target, t) :
            easeOutQuint(sprite.getPosition(), tween.target, t);
        
        sprite.setPosition(smooth);

        auto delta = (tween.rotation - sprite.getRotation()).wrapSigned();
        sprite.rotate(delta * (tween.mode == 0 ? smoothstep(0.0f,1.0f,t) : easeOutQuint(0.0f,1.0f,t)));

        if(tween.time > tween.duration){
            _data->registry.erase<TweenComponent>(entity);
            continue;
        }
    }
}

void TweenSystem::on_init() {
    
}