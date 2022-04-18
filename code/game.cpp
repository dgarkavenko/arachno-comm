#include "game.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include "drag_system.h"
#include <random>

#define CB(name) void (Game:: *name)(float dt)
typedef CB(update_f);

update_f misc_update;
static bool Running = true;

HandSystem *hand;
TweenSystem *tween;
DragSystem *drag;

float GAME_TIME = 0;
static sf::Vector2f card_scale_default (0.2f, 0.2f);
static sf::Vector2f card_scale_hover (0.25f, 0.25f);

static std::random_device rd;     // only used once to initialise (seed) engine
static std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)

Game::Game(int w, int h, std::string title) {
    _data = std::make_shared<GameData>();
    _data->window.create(sf::VideoMode(w,h), title);
    _data->window.setFramerateLimit(60);


    new ConstructorSystem(_data);
    new FPSCounter(_data);
    new HandSystem(_data);
    new TweenSystem(_data);
    new Debuger(_data);
    new DragSystem(_data);

    misc_update = &Game::update_misc;

    auto map = _data->registry.create();
    auto &map_blank = _data->registry.emplace<blanks::Sprite>(map, "../assets/images/map_ww.jpeg");
    map_blank.origin = {0,0};
    map_blank.on_create = [](sf::Sprite& sprite){

        std::uniform_int_distribution<int> random_x(0, sprite.getTexture()->getSize().x - RENDER_WIDTH); // guaranteed unbiased
        std::uniform_int_distribution<int> random_y(0, sprite.getTexture()->getSize().y - RENDER_HEIGHT); // guaranteed unbiased
        sprite.setTextureRect({
            {random_x(rng),random_y(rng)},
            {RENDER_WIDTH, RENDER_HEIGHT}
        });
    };

    for (size_t i = 0; i < 7; i++)
    {
        auto card = _data->registry.create();
        _data->registry.emplace<blanks::Sprite>(card, "../assets/images/card_back.png", card_scale_default);
        _data->registry.emplace<tag::Selectable>(card);
        _data->registry.emplace<tag::InHand>(card);
    }

    loop();

}

void Game::update_misc(float dt){

    _data->registry
        .view<sf::Sprite>(entt::exclude<tag::Hover>)
        .each([this](const sf::Sprite &sprite) {
            //sprite.setScale(card_scale_default);
            _data->window.draw(sprite);

        });

    _data->registry
        .view<sf::Sprite, tag::Hover>()
        .each([this](const sf::Sprite &sprite) {
            //sprite.setScale(card_scale_hover);
            _data->window.draw(sprite);
        });

    _data->registry
        .view<sf::Text>()
        .each([this](const sf::Text &text){_data->window.draw(text);});
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
        
        for (auto delegate : _data->delegates)
            delegate(dt);

        std::invoke(misc_update, this, dt);
        window.display();
        std::cout.flush();
        GAME_TIME += dt;
    }
}
