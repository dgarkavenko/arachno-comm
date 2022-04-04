#include "game.h"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Main.hpp>
#include <fmt/core.h>
#include <iostream>
#include <SFML/System/Angle.hpp>
#include "fps_counter.h"
#include "texture_loader_system.h"
#include "card_template.h"

sf::Font font;

Game::Game(int w, int h, std::string title) {
    _data = std::make_shared<GameData>();
    _data->window.create(sf::VideoMode(w,h), title);
    _data->window.setFramerateLimit(60);
    
    draw = new DrawSystem(_data);
    fps = new FPSCounter(_data);
    test_observer = new ObserverSystem(_data);

    new TextureLoader(_data);

    entt::registry & reg = _data->registry;

    for (size_t i = 0; i < 7; i++)
    {
        auto card = reg.create();
        reg.emplace<CardTemplateComponent>(card, "../assets/images/card_back.png", sf::Vector2f(0.2f,0.2f));
    };

    run();
}

void Game::update_systems(float dt){
    draw->update(dt);
    fps->update(dt);
    test_observer->update(dt);

    auto view = _data->registry.view<sf::Text>();
    for (entt::entity entity : view){

        sf::Text &text = view.get<sf::Text>(entity);
        _data->window.draw(text);
    }
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
