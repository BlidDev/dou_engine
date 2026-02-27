#include "components/luascript.h"
#include "log.h"
#include "luawrapper.h"

namespace engine {
    sol::state LuaManager::state;


    void LuaManager::init(const char* util_path) {
        state.open_libraries(sol::lib::string, sol::lib::base, sol::lib::coroutine, sol::lib::io, sol::lib::math, sol::lib::package);
        expose_env(state);
        if (!util_path) return;
        state.require_file("util", util_path);
    }

    LuaUpdate::LuaUpdate(UUID self, Scene* scene, sol::state& state, std::string path) {
        this->self = self;
        this->path = std::filesystem::path(path).filename();
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

        l_on_init = env["on_init"];
        l_on_update = env["on_update"];
        l_on_end = env["on_end"];
    }

    LuaUpdate::LuaUpdate() {
        env = nullptr;
        path = "";
        self = 0;
        inital_error = "";

        l_on_init = {};
        l_on_update = {};    
        l_on_end = {};
    }

    void LuaUpdate::on_init() {
        if (!l_on_init) return; // Doesn't exist
        auto result = l_on_init();
        if (!result.valid()) {
            sol::error e = result;
            DU_ERROR("{} {}", inital_error,e.what());
        }
    }

    void LuaUpdate::on_update(float dt) {
        if (!l_on_update) return; // Doesn't exist
        auto result = l_on_update(dt);

        if (!result.valid()) {
            sol::error e = result;
            DU_ERROR("{} {}", inital_error,e.what());
        }
    }

    void LuaUpdate::on_end() {
        if (!l_on_end) return; // Doesn't exist
        auto result = l_on_end();

        if (!result.valid()) {
            sol::error e = result;
            DU_ERROR("{} {}", inital_error,e.what());
        }
    }

    void LuaActionComp::remove(std::string path) {
        const auto& it = std::find_if(scripts.begin(), scripts.end(), [&path] (const auto& s){ return s.path == path; });

        DU_ASSERT(it == scripts.end(), "Trying to remove non registered script {}", path);
        scripts.erase(it);
    }


    bool LuaActionComp::find(const char* path) {
        return std::find_if(scripts.begin(), scripts.end(), [&path](const auto& s) {return s.path == path;}) != scripts.end();
    }

    LuaActionComp::LuaActionComp(UUID self) {
        this->self = self;
        scripts = {};
    }

    LuaActionComp::LuaActionComp(UUID self, const std::vector<LuaUpdate>& scripts) {
        this->self = self;
        this->scripts = scripts;
    }

    LuaActionComp& LuaActionComp::add(Scene* scene, std::string path) {
        if (find(path.c_str())) { DU_CORE_DEBUG_TRACE("{} is already attached to {}. Ignoring", path, self); return *this;}

        scripts.push_back(LuaUpdate(self, scene,LuaManager::state,scene->get_script(path.c_str())));
        return *this;
    }

    LuaActionComp& LuaActionComp::add(LuaUpdate update) {
        if (find(update.path.c_str())) { DU_CORE_DEBUG_TRACE("{} is already attached to {}. Ignoring", update.path, self); return *this;}
        scripts.push_back(update);
        return *this;
    }

    LuaUpdate& LuaActionComp::get_last() {
        
        return scripts.back();
    }

}
