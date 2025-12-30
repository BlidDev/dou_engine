#pragma once

#include "scene.h"
namespace engine {
    template <typename T>
    T& get_uuid_component(Scene* owner, UUID uuid) {
        return owner->get_uuid_component<T>(uuid);
    }

    template <typename T>
    bool has_uuid_component(Scene* owner, UUID uuid) {
        return owner->has_uuid_component<T>(uuid);
    }

    void expose_env_types(sol::state& env);


    void expose_env_functions(sol::state& env);

    void expose_env(sol::state& env);
}
