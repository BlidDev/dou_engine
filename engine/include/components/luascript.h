#pragma once
#include "components/uuid.h"
#include "scene.h"
#include <espch.h>

namespace engine {

    struct LuaManager {
        static void init();
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
       LuaActionComp(UUID self,std::vector<LuaUpdate> scripts);

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
            for (auto& s : scripts) {
                if (s.path != callback.path)
                    continue;
                sol::function fn = s.env[callback.function];
                EG_ASSERT(!fn, "The scripts {} does not contain {}", callback.path, callback.function);
                auto result = fn(std::forward<Args>(args)...);
                if (!result.valid()) {
                    sol::error e = result;
                    EG_ERROR("{}", e.what());
                }
                return (int)result;
            }

            EG_ASSERT(true, "No script ({}) attached to entity", callback.path);
            return -1;
        }

       LuaUpdate& get_last();

       std::vector<LuaUpdate> scripts;
       UUID self;
    };

}
