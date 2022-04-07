#pragma once
#include "base_system.h"
#include <SFML/Graphics.hpp>
#include <map>

class TextureLoader : public BaseSystem{
public:
    std::map<std::string, sf::Texture*> textures; 
    TextureLoader(GameDataRef data) : BaseSystem::BaseSystem(data){on_init();};
    void update(float dt) override;
protected:
    void on_init() override;
    sf::Texture* get_texture(std::string name);
    void on_template_consturct(entt::registry &registry, entt::entity entity);

};