#include "texture_loader_system.h"
#include "card_template.h"
#include <fmt/core.h>

void TextureLoader::update(float dt) {
    // auto view = _data->registry.view<CardTemplateComponent, sf::Sprite>();
    // for (auto enity : view) {
    //     _data->registry.erase<CardTemplateComponent>(enity);
    // };
}

void TextureLoader::on_template_consturct(entt::registry &registry, entt::entity entity)
{
    CardTemplateComponent& cardTemplate = registry.get<CardTemplateComponent>(entity);
    auto& sprite = registry.emplace<sf::Sprite>(entity);
    sprite.setTexture(*get_texture(cardTemplate.texture_path));
    sprite.setScale(cardTemplate.scale);
    const auto& bounds = sprite.getLocalBounds();
    sprite.setOrigin({bounds.width / 2.0f, bounds.height / 2.0f});
    registry.emplace<InHand>(entity);
}


void TextureLoader::on_init() {
    _data->registry.on_construct<CardTemplateComponent>().connect<&TextureLoader::on_template_consturct>(this);
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
