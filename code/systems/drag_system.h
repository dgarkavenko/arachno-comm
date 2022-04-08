#pragma once
#include "game_data.h"
#include "components.h"

class DragSystem
{
public:
    DragSystem(GameDataRef data) : _data(data){init();};
    void update(float dt);
    void hover(const sf::Vector2f mouse_position);
    void drag(const sf::Vector2f mouse_position);
    void inertia(const sf::Vector2f mouse_position);

protected:
    GameDataRef _data;
    void init();
};
