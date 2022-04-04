#include "game.h"

#include <SFML/Graphics.hpp>
#include <iostream>
#include "hand_system.h"

#define CB(name) void (Game:: *name)(float dt)
typedef CB(update_f);


update_f systems_update;
static bool Running = true;

HandSystem *hand;

Game::Game(int w, int h, std::string title) {
    _data = std::make_shared<GameData>();
    _data->window.create(sf::VideoMode(w,h), title);
    _data->window.setFramerateLimit(60);
    
    draw = new DrawSystem(_data);
    fps = new FPSCounter(_data);

    hand = new HandSystem(_data);

    new TextureLoader(_data);

    entt::registry &reg = _data->registry;

    for (size_t i = 0; i < 8; i++)
    {
        auto card = reg.create();
        reg.emplace<CardTemplateComponent>(card, "../assets/images/card_back.png", sf::Vector2f(0.2f,0.2f));
    };

    systems_update = &Game::update_systems;
    loop();
    
}

void Game::update_systems(float dt){
    draw->update(dt);
    fps->update(dt);
    hand->update(dt);

    auto view = _data->registry.view<sf::Text>();
    for (entt::entity entity : view){

        sf::Text &text = view.get<sf::Text>(entity);
        _data->window.draw(text);
    }
}

void Game::loop() {

    sf::RenderWindow &window = _data->window;
    while (window.isOpen())
    {
        float dt = _clock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed)
                window.close();
        
        window.clear();       
        std::invoke(systems_update, this, dt);
        window.display();
        std::cout.flush();
    }
}
