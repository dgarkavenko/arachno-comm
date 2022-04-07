#include "font_system.h"


FontSystem::FontSystem(GameDataRef data){
    _data = data;
    _data->registry.on_construct<blanks::Text>().connect<&FontSystem::text_construct>(this);
}


void FontSystem::text_construct(entt::registry &registry, entt::entity entity)
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

sf::Font *FontSystem::get_font(std::string path)
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
