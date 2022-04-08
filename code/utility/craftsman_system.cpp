#include "craftsman_system.h"


void CraftsmanSystem::init() {
    _data->registry.on_construct<blanks::Text>().connect<&CraftsmanSystem::text_construct>(this);
    _data->registry.on_construct<blanks::Sprite>().connect<&CraftsmanSystem::on_template_consturct>(this);
}

void CraftsmanSystem::text_construct(entt::registry &registry, entt::entity entity)
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


void CraftsmanSystem::on_template_consturct(entt::registry &registry, entt::entity entity)
{
    blanks::Sprite& blank = registry.get<blanks::Sprite>(entity);
    auto& sprite = registry.emplace<sf::Sprite>(entity);
    sprite.setTexture(*get_texture(blank.texture_path));
    sprite.setScale(blank.scale);
    const auto& bounds = sprite.getLocalBounds();
    sprite.setOrigin({bounds.width / 2.0f, bounds.height / 2.0f});
    registry.emplace<tag::InHand>(entity);
}

sf::Font *CraftsmanSystem::get_font(std::string path)
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

sf::Texture* CraftsmanSystem::get_texture(std::string name) {
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