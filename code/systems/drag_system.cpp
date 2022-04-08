#include "drag_system.h"
#include <SFML/Window.hpp>
#include <random>

std::random_device rd;     // only used once to initialise (seed) engine
std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)


sf::Vector2f last_position;

void DragSystem::update(float dt) {
    
    const sf::Vector2f localPosition = (sf::Vector2f) sf::Mouse::getPosition(_data->window); // window is a sf::Window
    const sf::Vector2f delta = localPosition - last_position;

    hover(localPosition);
    drag(delta);
    inertia(delta);
    
    last_position = localPosition;
}

void DragSystem::hover(sf::Vector2f mouse_position) {

    if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
        return;

    _data->registry.clear<tag::Hover>();

    auto view = _data->registry.view<sf::Sprite>();
    entt::entity tag = entt::null;

    for (entt::entity entity : view)
    {
        auto &sprite = view.get<sf::Sprite>(entity);
        if (sprite.getGlobalBounds().contains(mouse_position))
            tag = entity;
    }

    if (tag != entt::null)
        _data->registry.emplace<tag::Hover>(tag);
}

void DragSystem::drag(sf::Vector2f delta) {

    if(!sf::Mouse::isButtonPressed(sf::Mouse::Left))
        return;

    auto view = _data->registry.view<tag::Hover, sf::Sprite>();
    for (auto entity : view)
    {
        auto &sprite = view.get<sf::Sprite>(entity);
        auto new_position = sprite.getPosition() + delta;
        sprite.setPosition(new_position);
        _data->registry.remove<TweenComponent>(entity);
        _data->registry.emplace_or_replace<tag::Drag>(entity);
    }
}

void DragSystem::inertia(sf::Vector2f delta) {
    
    if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
        return;

    auto view = _data->registry.view<tag::Drag, sf::Sprite>();
    for (auto entity : view){
        auto &sprite = view.get<sf::Sprite>(entity);
        
        if(delta.length() > 1){

            int ln = delta.length() * 2;

            std::uniform_int_distribution<int> uni(-ln,ln); // guaranteed unbiased
            auto random_integer = uni(rng);

            auto &tween = _data->registry.emplace_or_replace<TweenComponent>(entity);
            tween.target = sprite.getPosition() + delta * 8.0f;
            tween.duration = delta.length() * 0.07f;
            tween.rotation = sprite.getRotation() + sf::degrees(random_integer);
            tween.mode = 1;
            tween.time = 0;
        }

        _data->registry.erase<tag::Drag>(entity);

    }
}

void DragSystem::init() {
    
}
