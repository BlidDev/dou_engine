#include "systems/luasys.h"
#include "scene.h"
#include "components/luascript.h"

namespace engine {

    void lua_action_init(Scene* scene) {

        auto actions = scene->registry.view<LuaActionComp>();

        for (auto [entity, actns] : actions.each()) {
            for (auto& act : actns.scripts) {
                act.on_init();
            }
        }
    }

    void lua_action_update(Scene* scene, float dt) {

        auto actions = scene->registry.view<LuaActionComp>();

        for (auto [entity, actns] : actions.each()) {
            for (auto& act : actns.scripts) {
                act.on_update(dt);
            }
        }
    }

    void lua_action_end(Scene* scene) {

        auto actions = scene->registry.view<LuaActionComp>();

        for (auto [entity, actns] : actions.each()) {
            for (auto& act : actns.scripts) {
                act.on_end();
            }
        }
    }
}
