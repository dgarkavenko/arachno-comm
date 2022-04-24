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
static const sf::Vector2i CARD_SIZE {145, 208};


static std::random_device rd;     // only used once to initialise (seed) engine
static std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)

float random(float LO, float HI){
    return LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
}

Game::Game(int w, int h, std::string title) {
    _data = std::make_shared<GameData>();
    _data->window.create(sf::VideoMode(w,h), title);
    _data->window.setFramerateLimit(60);


    new ConstructorSystem(_data);
    new HandSystem(_data);
    new TweenSystem(_data);
    new DragSystem(_data);

    new FPSCounter(_data);
    new Debuger(_data);

    misc_update = &Game::update_misc;


    auto back_entity = _data->registry.create();
    _data->registry.emplace<blanks::Sprite>(back_entity, "../assets/images/card_back.png", card_scale_default);
    sf::Sprite *back_sprite = &_data->registry.get<sf::Sprite>(back_entity);

    for (size_t i = 0; i < 7; i++)
    {
        auto card_entity = _data->registry.create();
        blanks::Sprite card_blank {};
        card_blank.texture_path = "../assets/images/irish_dudes.jpeg";

        card_blank.on_create = [](sf::Sprite& sprite){

            float zoom_out = random(1.5f, 2.5f);
            sprite.setScale({1.0f/zoom_out, 1.0f/zoom_out});

            std::uniform_int_distribution<int> random_x(0, sprite.getTexture()->getSize().x - (int)(CARD_SIZE.x * zoom_out)); // guaranteed unbiased
            std::uniform_int_distribution<int> random_y(0, sprite.getTexture()->getSize().y - (int)(CARD_SIZE.y * zoom_out)); // guaranteed unbiased
            sprite.setTextureRect({
                {random_x(rng),random_y(rng)},
                {(int)(CARD_SIZE.x * zoom_out), (int)(CARD_SIZE.y * zoom_out)}
            });

            const auto& bounds = sprite.getLocalBounds();
            sprite.setOrigin({bounds.width * .5f, bounds.height * .5f});
        };

        _data->registry.emplace<blanks::Sprite>(card_entity, card_blank);
        _data->registry.emplace<tag::Selectable>(card_entity);
        _data->registry.emplace<tag::InHand>(card_entity);
        
        auto &card = _data->registry.emplace<Card>(card_entity);
        card.face = &_data->registry.get<sf::Sprite>(card_entity);
        card.back = back_sprite;

    }

    auto map = _data->registry.create();

    blanks::Sprite map_blank {};
    map_blank.texture_path = "../assets/images/map_ww.jpeg";
    map_blank.origin = {0,0};
    map_blank.on_create = [](sf::Sprite& sprite){
        std::uniform_int_distribution<int> random_x(0, sprite.getTexture()->getSize().x - RENDER_WIDTH); // guaranteed unbiased
        std::uniform_int_distribution<int> random_y(0, sprite.getTexture()->getSize().y - RENDER_HEIGHT); // guaranteed unbiased
        sprite.setTextureRect({
            {random_x(rng),random_y(rng)},
            {RENDER_WIDTH, RENDER_HEIGHT}
        });
    };
    
    _data->registry.emplace<blanks::Sprite>(map, map_blank);

    loop();
}

void Game::update_misc(float dt){

    _data->registry
        .view<Card, sf::Sprite>(entt::exclude<tag::Hover>)
        .each([this](Card &card, const sf::Sprite &sprite) {
            auto& view = *card.up();
            view.setPosition(sprite.getPosition());
            view.setRotation(sprite.getRotation());
            _data->window.draw(view);
        });

    _data->registry
        .view<Card, sf::Sprite, tag::Hover>()
        .each([this](Card &card, const sf::Sprite &sprite) {
            auto& view = *card.up();
            view.setPosition(sprite.getPosition());
            view.setRotation(sprite.getRotation());
            _data->window.draw(view);
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
