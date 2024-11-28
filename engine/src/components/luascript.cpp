#include "components/luascript.h"
#include "log.h"
#include "luawrapper.h"

namespace engine {
    sol::state LuaManager::state;

    void LuaManager::init() {
        state.open_libraries(sol::lib::string, sol::lib::base, sol::lib::coroutine, sol::lib::io, sol::lib::math);
        expose_env(state);
        state.require_file("util", "res/scripts/util.lua");
    }

    LuaUpdate::LuaUpdate(UUID self, Scene* scene, sol::state& state, std::string path) {
        this->self = self;
        this->path = path;
        inital_error = "";
        env = sol::environment(state,sol::create, state.globals());
        auto result = state.safe_script_file(path, env, &sol::script_pass_on_error);
        if (!result.valid()) {
            sol::error e = result;
            inital_error = e.what();
            EG_CORE_ERROR("{}", inital_error);
        }
        env["util"] = state["util"];
        env["this"] = self;
        env["scene"] = scene;
    }

    void LuaUpdate::on_init() {
        sol::protected_function init = env["on_init"];
        auto result = init();
        if (!result.valid()) {
            sol::error e = result;
            EG_ERROR("{} {}", inital_error,e.what());
        }
    }

    void LuaUpdate::on_update(float dt) {
        sol::protected_function update = env["on_update"];
        auto result = update(dt);
        if (!result.valid()) {
            sol::error e = result;
            EG_ERROR("{} {}", inital_error,e.what());
        }
    }

    void LuaUpdate::on_end() {
        sol::protected_function end = env["on_end"];
        auto result = end();
        if (!result.valid()) {
            sol::error e = result;
            EG_ERROR("{} {}", inital_error,e.what());
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
