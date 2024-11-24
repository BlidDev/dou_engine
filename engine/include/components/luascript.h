#pragma once
#include "components/uuid.h"
#include "scene.h"
#include <sol.hpp>
#include <string>
#include <vector>

namespace engine {

    struct LuaManager {
        static void init();
        static sol::state state;
    };

    struct LuaUpdate {
        LuaUpdate(UUID self, Scene* scene, sol::state& state, std::string path);
        void on_init();
        void on_update(float dt);
        void on_end();

        sol::environment env;
        std::string path;
        UUID self;
    };

    struct LuaActionComp {
       LuaActionComp(UUID self);
       LuaActionComp(UUID self,std::vector<LuaUpdate> scripts);

       LuaActionComp& add(Scene* scene, std::string path);
       LuaActionComp& add(LuaUpdate update);

       LuaUpdate& get_last();

       std::vector<LuaUpdate> scripts;
       UUID self;
    };

}
