#include "components/luascript.h"
#include "scene.h"
#include <espch.h>


namespace engine {


    struct BoxColliderComp {
        BoxColliderComp() {
            size_bounded = true;
            is_solid = false; size = glm::vec3(0.0f);
            intersects_callback = nullptr;
            lua_callback = {"", ""};
        }
        bool size_bounded;
        bool is_solid;
        glm::vec3 size;

        int (*intersects_callback)(Scene&, entt::entity, entt::entity) = nullptr;
        LuaCallback lua_callback;
    };

}
