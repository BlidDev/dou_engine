#include "luawrapper.h"
#include <espch.h>

namespace engine {
    void expose_env(sol::state& env){
        expose_env_types(env);
        expose_env_functions(env);
    }
}
