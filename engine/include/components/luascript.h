#pragma once
#include "components/uuid.h"
#include "scene.h"
#include <espch.h>

namespace engine {

    struct LuaManager {
        static void init(const char* util_path = nullptr);
        static sol::state state;
    };

    struct LuaCallback {
        std::string path = "";
        std::string function = "";

        operator bool() const {
            return (!path.empty() && !function.empty());
        }
    };

    struct LuaUpdate {
        LuaUpdate(UUID self, Scene* scene, sol::state& state, std::string path);
        LuaUpdate();
        void on_init();
        void on_update(float dt);
        void on_end();

        sol::environment env;
        std::string path;
        UUID self;
        std::string inital_error;
    private:
    };

    struct LuaActionComp {
       LuaActionComp(UUID self);
       LuaActionComp(UUID self, const std::vector<LuaUpdate>& scripts);

        template<typename ...Args>
        LuaActionComp(UUID self, Args&&... args) {
            this->self = self;
            scripts = {std::forward<Args>(args)...};
        }

       LuaActionComp& add(Scene* scene, std::string path);
       LuaActionComp& add(LuaUpdate update);

       void remove(std::string path);
       bool find(const char* path);

       template<typename T>
       LuaActionComp& bind_field(std::string name, T value) {
           scripts.back().env[name] = value;
           return *this;
       }

        template<typename ...Args>
        int call_at(const LuaCallback& callback, Args&&... args) {
            auto script = std::find_if(scripts.begin(), scripts.end(), [&callback](const auto& s) {return s.path == callback.path;});
            DU_ASSERT(script == scripts.end(), "No script ({}) attached to entity", callback.path);


            
            sol::function fn = script->env[callback.function];
            DU_ASSERT(!fn, "The scripts {} does not contain {}", callback.path, callback.function);

            auto result = fn(std::forward<Args>(args)...);
            if (!result.valid()) {
                sol::error e = result;
                DU_ERROR("{}", e.what());
            }
            return (int)result;
        }

       LuaUpdate& get_last();

       std::vector<LuaUpdate> scripts;
       UUID self;
    };

}
