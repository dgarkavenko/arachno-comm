#include "draw_system.h"
#include "SFML/Graphics.hpp"

void DrawSystem::update (float dt) {
    
    auto view = _data->registry.view<sf::Sprite>();
    view.each([this](const sf::Sprite &sprite) {
        _data->window.draw(sprite);
    });
}