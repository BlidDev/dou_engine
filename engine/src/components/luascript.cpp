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
            DU_CORE_ERROR("{}", inital_error);
        }
        env["util"] = state["util"];
        env["this"] = self;
        env["scene"] = scene;
    }

    void LuaUpdate::on_init() {
        sol::protected_function init = env["on_init"];
        if (!init) // does not exist
            return;
        auto result = init();
        if (!result.valid()) {
            sol::error e = result;
            DU_ERROR("{} {}", inital_error,e.what());
        }
    }

    void LuaUpdate::on_update(float dt) {
        sol::protected_function update = env["on_update"];
        if (!update) // does not exist
            return;
        auto result = update(dt);
        if (!result.valid()) {
            sol::error e = result;
            DU_ERROR("{} {}", inital_error,e.what());
        }
    }

    void LuaUpdate::on_end() {
        sol::protected_function end = env["on_end"];
        if (!end) // does not exist
            return;
        auto result = end();
        if (!result.valid()) {
            sol::error e = result;
            DU_ERROR("{} {}", inital_error,e.what());
        }
    }

    void LuaActionComp::remove(std::string path) {
        int i = 0; bool found = false;
        for (auto& s : scripts) {
            if (s.path == path) {found = true; break;}
            i++;
        }

        DU_ASSERT(!found, "Trying to remove non registered script {}", path);
        scripts.erase(scripts.begin() + i);
    }


    bool LuaActionComp::find(const char* path) {
        for (auto& s : scripts) {
            if (s.path == path)
                return true;
        }
        return false;
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
