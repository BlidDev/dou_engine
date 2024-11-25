#include "components/luascript.h"
#include "log.h"
#include "luawrapper.h"
#include "sol.hpp"

namespace engine {
    sol::state LuaManager::state;

    void LuaManager::init() {
        state.open_libraries(sol::lib::string, sol::lib::base, sol::lib::coroutine, sol::lib::io, sol::lib::math);
    }

    LuaUpdate::LuaUpdate(UUID self, Scene* scene, sol::state& state, std::string path) {
        this->self = self;
        this->path = path;
        env = sol::environment(state,sol::create, state.globals());
        state.safe_script_file(path, env, &sol::script_pass_on_error);
        expose_env(env);
        env["this"] = self;
        env["scene"] = scene;
    }

    void LuaUpdate::on_init() {
        sol::protected_function init = env["on_init"];
        if (!init) // does exist
            return;
        auto result = init();
        if (!result.valid()) {
            sol::error e = result;
            EG_ERROR("{}", e.what());
        }
    }

    void LuaUpdate::on_update(float dt) {
        sol::protected_function update = env["on_update"];
        if (!update) // does exist
            return;
        auto result = update(dt);
        if (!result.valid()) {
            sol::error e = result;
            EG_ERROR("{}", e.what());
        }
    }

    void LuaUpdate::on_end() {
        sol::protected_function end = env["on_end"];
        if (!end) // does exist
            return;
        auto result = end();
        if (!result.valid()) {
            sol::error e = result;
            EG_ERROR("{}", e.what());
        }
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
