#include <sol.hpp>
#include "component.h"

namespace engine {
    void expose_env_types(sol::environment& env);
    TransformComp& get_uuid_transfrom(Scene* owner, UUID uuid);

    void expose_env_functions(sol::environment& env);

    void expose_env(sol::environment& env);
}
