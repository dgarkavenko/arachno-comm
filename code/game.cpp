#include "game.h"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Main.hpp>
#include <fmt/core.h>
#include <iostream>
#include <SFML/System/Angle.hpp>
#include "fps_counter.h"
#include "observer_system.h"


sf::Font font;
ObserverSystem _os;


void sf_text_construct(entt::registry &registry, entt::entity entity){
    sf::Text &text = registry.get<sf::Text>(entity);
    text.setFont(font);
    text.setCharacterSize(18);
    text.setFillColor(sf::Color::Red);
}

Game::Game(int w, int h, std::string title) {
    _data = std::make_shared<GameData>();
    _data->window.create(sf::VideoMode(w,h), title);
    _data->window.setFramerateLimit(60);
    
    _draw.init(_data);
    _fps.init(_data);
    _os.init(_data);

    bool loaded = font.loadFromFile("../assets/PT_Sans/PTSans-Regular.ttf");

    entt::registry & reg = _data->registry;
    auto entity = reg.create();
    reg.on_construct<sf::Text>().connect<&sf_text_construct>();

    reg.emplace<FPSCounterComponent>(entity);

    const int numbers[10] = {1,2,3,4,5,6,7,8,9,10};
    for (auto element : numbers)
        reg.emplace<PickleComponent>(reg.create(), element * 100);
    
    sf::Texture texture;
    if (!texture.loadFromFile("../assets/images/card_back.png"))
        return;

    texture.setSmooth(false);

    for (size_t i = 0; i < 7; i++)
    {
        auto card = reg.create();
        auto& sprite = reg.emplace<sf::Sprite>(card);
        sprite.setTexture(texture);
        sprite.setScale({0.15f, 0.15f});
        sprite.setRotation(sf::degrees(0));
        sf::FloatRect bounds = sprite.getLocalBounds();
        sprite.setPosition({bounds.width * i * sprite.getScale().x, 20});
    };

    run();
}

void Game::update_systems(float dt){
    _draw.update(dt);
    _fps.update(dt);
    _os.update(dt);

    auto view = _data->registry.view<sf::Text>();
    for (auto entity : view)
        _data->window.draw(view.get<sf::Text>(entity));
}

void Game::run() {

    sf::RenderWindow &window = _data->window;
    while (window.isOpen())
    {
        float dt = _clock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed)
                window.close();
        
        window.clear();       
        update_systems(dt);
        window.display();
        std::cout.flush();
    }
}
