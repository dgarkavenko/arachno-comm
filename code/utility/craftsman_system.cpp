#include "craftsman_system.h"


void ConstructorSystem::init() {

    update_delegate delegate{};
    delegate.connect<&ConstructorSystem::update>(this);
    _data->delegates.push_back(delegate);
    
    //_data->registry.on_construct<blanks::Text>().connect<&CraftsmanSystem::text_construct>(this);
    //_data->registry.on_construct<blanks::Sprite>().connect<&CraftsmanSystem::sprite_construct>(this);
}

void ConstructorSystem::update(float dt){

    {
        auto view = _data->registry.view<blanks::Text>();
        for (auto entity : view)
            text_construct(_data->registry, entity);
    }

    {
        auto view = _data->registry.view<blanks::Sprite>();
        for (auto entity : view)
            sprite_construct(_data->registry, entity);
    }

    _data->registry.clear<blanks::Text>();
    _data->registry.clear<blanks::Sprite>();
}

void ConstructorSystem::text_construct(entt::registry &registry, entt::entity entity)
{
    blanks::Text &blank = registry.get<blanks::Text>(entity);
    sf::Text *text = &registry.emplace_or_replace<sf::Text>(entity);

    text->setFont(*get_font(blank.font));
    text->setCharacterSize(blank.size);
    text->setFillColor(blank.color);
    text->setString(blank.text);

    if(blank.bind_ptr != nullptr)
        *blank.bind_ptr = text;
}


void ConstructorSystem::sprite_construct(entt::registry &registry, entt::entity entity)
{
    blanks::Sprite& blank = registry.get<blanks::Sprite>(entity);
    auto& sprite = registry.emplace<sf::Sprite>(entity);
    sprite.setTexture(*get_texture(blank.texture_path));
    sprite.setScale(blank.scale);

    if(blank.origin.lengthSq() > 0){
        const auto& bounds = sprite.getLocalBounds();
        sprite.setOrigin({bounds.width * blank.origin.x, bounds.height * blank.origin.y});
    }

    if(blank.on_create)
        blank.on_create(sprite);
}

sf::Font *ConstructorSystem::get_font(std::string path)
{
    if (!fonts.count(path))
    {
        auto font = new sf::Font();
        if (font->loadFromFile(fmt::format("../assets/{0}", path)))
        {
            fonts[path] = font;
            return font;
        }
        else
        {
            return nullptr;
        }
    }

    return fonts[path];
}

sf::Texture* ConstructorSystem::get_texture(std::string name) {
    if( !textures.count(name)){
         auto texture = new sf::Texture();
        if(texture->loadFromFile(name)){
            textures[name] = texture;
            return texture;
        }
        else{
            return nullptr;
        }

    }
    
    return textures[name];
}