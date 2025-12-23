#include "systems/actionsys.h"
#include "components/action.h"
#include "entity.h"


namespace engine {
    void actions_init(Scene* scene) {
        auto actions = scene->registry.view<ActionsComp>();

        for (auto [entity, actns] : actions.each()) {
            for (ActionsComp::UpdatePtr& act : actns.actions) {
               Entity e = {scene, entity};
                act->on_init(scene, e);
            }
        }
    }

    void actions_update(Scene* scene, float dt) {
        auto actions = scene->registry.view<ActionsComp>();

        for (auto [entity, actns] : actions.each()) {
            for (ActionsComp::UpdatePtr& act : actns.actions) {
               Entity e = {scene, entity};
                act->on_update(scene, e, dt);
            }
        }
    }


    void actions_end(Scene* scene) {
        auto actions = scene->registry.view<ActionsComp>();

        for (auto [entity, actns] : actions.each()) {
            for (ActionsComp::UpdatePtr& act : actns.actions) {
               Entity e = {scene, entity};
                act->on_end(scene, e);
            }
        }
    }
}
