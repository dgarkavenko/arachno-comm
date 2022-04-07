#include "hand_system.h"
#include <fmt/core.h>
#include <SFML/Graphics.hpp>
#include <math.h>
#include "blanks.h"
#include "components.h"


static float overlap = .4f;
static float radius = 180;
static float vertical_offset = 500;
static float length = 2.f;
static float rotation_pivot = 1200;
static float flatten = 2.f;
//https://www.desmos.com/calculator/d1upvx1kjj?lang=ru

float get_arc_y(float x)
{
    x = (x - RENDER_WIDTH / 2.f) / length;
    float y = -(sqrt(radius * radius - x * x) - RENDER_HEIGHT) + vertical_offset;
    return y / flatten;
}

sf::Angle get_arc_rotation(const sf::Vector2f& target){
    
    sf::Vector2f pivot {RENDER_WIDTH / 2.f, rotation_pivot};
    auto dir = target - pivot;
    return dir.normalized().angle();
}

void HandSystem::update(float dt) {
    for (auto entity : observer){
        set_in_hand_positions();
        break;
    }
    observer.clear();
}

void HandSystem::on_init() {
    observer.connect(_data->registry, entt::collector.group<InHand, blanks::Sprite>());
}

sf::Vector2f real_size(const sf::Sprite& sprite){
    const sf::FloatRect &bounds = sprite.getLocalBounds();
    return {bounds.width * sprite.getScale().x, bounds.height * sprite.getScale().y};
}

void HandSystem::set_in_hand_positions() {

    auto view = _data->registry.view<const InHand, sf::Sprite>();
    
    int size_hint = view.size_hint();
    float placement = 0;
    int i = 0;

    for (auto entity : view) {
        auto& sprite = _data->registry.get<sf::Sprite>(entity);
        sf::Vector2f size = real_size(sprite);
        const float card_visible_size = (1 - overlap) * size.x;
        const float start_x = RENDER_WIDTH / 2.f - card_visible_size * ((size_hint - 1) / 2.f);
        float x = start_x + i * card_visible_size;

        TweenComponent& tween =
            _data->registry.emplace_or_replace<TweenComponent>(entity);

        tween.duration = 3;
        tween.time = 0;
        tween.target = {x, get_arc_y(x)};
        tween.rotation = get_arc_rotation(sprite.getPosition()) +  sf::degrees(90);
        i++;
    };
}
