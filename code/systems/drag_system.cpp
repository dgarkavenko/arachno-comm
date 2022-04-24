#include "drag_system.h"
#include <SFML/Window.hpp>
#include <fmt/core.h>


sf::Vector2f last_position;


void DragSystem::init() {
    _data->delegates.push_back({entt::connect_arg<&DragSystem::update>, this});
}


void DragSystem::update(float dt) {
    
    const sf::Vector2f localPosition = (sf::Vector2f) sf::Mouse::getPosition(_data->window); // window is a sf::Window
    const sf::Vector2f md = localPosition - last_position;

    hover(localPosition);

    if(sf::Mouse::isButtonPressed(sf::Mouse::Right)){
        auto view = _data->registry.view<tag::Hover, Card>();
        for (auto entity : view)
            view.get<Card>(entity).is_revealed = true;
    }

    drag(md);
    
    last_position = localPosition;
}


bool contains(const sf::Sprite sprite, const sf::Vector2f mouse_position){
            auto mouse_local = sprite.getTransform().getInverse().transformPoint(mouse_position);

        if (mouse_local.x > 0 &&
            mouse_local.y > 0 &&
            mouse_local.x < sprite.getLocalBounds().width &&
            mouse_local.y < sprite.getLocalBounds().height){
            return true;
        }

        return false;
}

void DragSystem::hover(sf::Vector2f mouse_position) {

    if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
        return;

    auto view = _data->registry.view<sf::Sprite, tag::Selectable>();
    entt::entity tag = entt::null;

    for (entt::entity entity : view)
    {
        auto &sprite = view.get<sf::Sprite>(entity);
        if(!contains(sprite, mouse_position))
            continue;
            
        if(_data->registry.all_of<tag::Hover>(entity))
            return;
        
        tag = entity;
    }

    _data->registry.clear<tag::Hover>();

    if (tag != entt::null)
        _data->registry.emplace<tag::Hover>(tag);
}

void DragSystem::drag(sf::Vector2f delta) {

    if(!sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        _data->registry.clear<tag::Drag>();
        return;
    }

    auto hover_only = _data->registry.view<tag::Hover>(entt::exclude<Inertia>);
    for (auto entity : hover_only)
        _data->registry.emplace_or_replace<Inertia>(entity);

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
        }

        _data->registry.erase<tag::Drag>(entity);
    }
}