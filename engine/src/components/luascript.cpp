#include "components/luascript.h"
#include "log.h"
#include "luawrapper.h"

namespace engine {
    sol::state LuaManager::state;

    void LuaManager::init() {
        state.open_libraries(sol::lib::base, sol::lib::coroutine, sol::lib::io, sol::lib::math);
    }

    LuaUpdate::LuaUpdate(UUID self, Scene* scene, sol::state& state, std::string path) {
        this->self = self;
        this->path = path;
        env = sol::environment(state,sol::create);
        state.script_file(path, env);
        expose_env(env);
        env["this"] = self;
        env["owner"] = scene;
        on_init();
    }

    void LuaUpdate::on_init() {
        auto init = env["on_init"];
        if (!init.valid() || !init.is<sol::function>())
            return;
        init();
    }

    void LuaUpdate::on_update(float dt) {
        auto update = env["on_update"];
        if (!update.valid() || !update.is<sol::function>())
            return;
        update(dt);
    }

    void LuaUpdate::on_end() {
        auto end = env["on_end"];
        if (!end.valid() || !end.is<sol::function>())
            return;
        end();
    }


    LuaActionComp::LuaActionComp(UUID self) {
        this->self = self;
        scripts = {};
    }

    LuaActionComp::LuaActionComp(UUID self, std::vector<LuaUpdate> scripts) {
        this->self = self;
        this->scripts = scripts;
    }

    LuaActionComp& LuaActionComp::add(Scene* scene, std::string path) {
        scripts.push_back(LuaUpdate(self, scene,LuaManager::state,path));
        EG_CORE_INFO("Added script {0}", path);
        return *this;
    }

    LuaActionComp& LuaActionComp::add(LuaUpdate update) {
        scripts.push_back(update);
        return *this;
    }

    LuaUpdate& LuaActionComp::get_last() {
        return scripts.back();
    }
}
