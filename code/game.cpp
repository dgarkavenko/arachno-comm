#include "game.h"

#include <SFML/Graphics.hpp>
#include <iostream>


#define CB(name) void (Game:: *name)(float dt)
typedef CB(update_f);

update_f systems_update;
static bool Running = true;

HandSystem *hand;
TweenSystem *tween;
Debuger *debuger;

float GAME_TIME = 0;


Game::Game(int w, int h, std::string title) {
    _data = std::make_shared<GameData>();
    _data->window.create(sf::VideoMode(w,h), title);
    _data->window.setFramerateLimit(60);

    new TextureLoader(_data);
    new FontSystem(_data);
    
    fps = new FPSCounter(_data);
    hand = new HandSystem(_data);
    tween = new TweenSystem(_data);
    debuger = new Debuger(_data);

    systems_update = &Game::update_systems;
    loop();

}


float next_spawn = 1;
float cards_to_spawn = 7;

void Game::update_systems(float dt){

    fps->update(dt);
    hand->update(dt);
    tween->update(dt);
    debuger->update();

    _data->registry
        .view<sf::Sprite>()
        .each([this](const sf::Sprite &sprite) {_data->window.draw(sprite);});

    _data->registry
        .view<sf::Text>()
        .each([this](const sf::Text &text){_data->window.draw(text);});

    _data->registry.clear<blanks::Text>();
    _data->registry.clear<blanks::Sprite>();

    //TODO(dg): move this logic

    if (GAME_TIME > next_spawn && cards_to_spawn > 0)
    {
        cards_to_spawn--;
        next_spawn = GAME_TIME + 1.0f;
        auto card = _data->registry.create();
        _data->registry.emplace<blanks::Sprite>(card, "../assets/images/card_back.png", sf::Vector2f(0.2f, 0.2f));
    }

    debuger->update();

}

void Game::loop() {


    sf::Clock clock;

    sf::RenderWindow &window = _data->window;
    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();
        
        sf::Event event;
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed)
                window.close();
        
        window.clear();       
        std::invoke(systems_update, this, dt);
        window.display();
        std::cout.flush();
        GAME_TIME += dt;
    }
}
