#include "texture_loader_system.h"
#include "blanks.h"
#include "components.h"
#include <fmt/core.h>

void TextureLoader::update(float dt) {
    // auto view = _data->registry.view<CardTemplateComponent, sf::Sprite>();
    // for (auto enity : view) {
    //     _data->registry.erase<CardTemplateComponent>(enity);
    // };
}

void TextureLoader::on_template_consturct(entt::registry &registry, entt::entity entity)
{
    blanks::Sprite& blank = registry.get<blanks::Sprite>(entity);
    auto& sprite = registry.emplace<sf::Sprite>(entity);
    sprite.setTexture(*get_texture(blank.texture_path));
    sprite.setScale(blank.scale);
    const auto& bounds = sprite.getLocalBounds();
    sprite.setOrigin({bounds.width / 2.0f, bounds.height / 2.0f});
    registry.emplace<InHand>(entity);
}


void TextureLoader::on_init() {
    _data->registry.on_construct<blanks::Sprite>().connect<&TextureLoader::on_template_consturct>(this);
}

sf::Texture* TextureLoader::get_texture(std::string name) {
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
