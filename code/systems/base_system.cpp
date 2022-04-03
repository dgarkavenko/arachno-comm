#include "base_system.h"


void BaseSystem::update(float dt) {
    
}

void BaseSystem::init(GameDataRef ref){ 
    _data = ref;
    on_init();
};

void BaseSystem::on_init() {
    
}
