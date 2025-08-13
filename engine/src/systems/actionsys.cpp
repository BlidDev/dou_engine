#include "systems/actionsys.h"
#include "components/action.h"
#include "entity.h"


namespace engine {

    void actions(Scene* scene, float dt) {
        auto actions = scene->registry.view<ActionsComp>();

        for (auto [entity, actns] : actions.each()) {
            for (auto& act : actns.actions) {
                Entity e = {scene, entity};
                act->on_update(scene, e, dt);
            }
        }
    }

    void end_actions(entt::registry& registry) {
        auto actions = registry.view<ActionsComp>();

        for (auto [_, act] : actions.each()) {
            for (auto* u : act.actions) {
                delete u;
            }
        }
    }

}
