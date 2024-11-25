#pragma once
#include "component.h"
#include "log.h"

namespace engine {
    template <typename T>
    T& get_uuid_component(Scene* owner, UUID uuid) {
        return owner->get_uuid_component<T>(uuid);
    }

    void expose_env_types(sol::environment& env);


    void expose_env_functions(sol::environment& env);

    void expose_env(sol::environment& env);
}
