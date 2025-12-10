#include "systems/actionsys.h"
#include "components/action.h"
#include "entity.h"


namespace engine {

    void actions(Scene* scene, float dt) {
        auto actions = scene->registry.view<ActionsComp>();

        for (auto [entity, actns] : actions.each()) {
            for (ActionsComp::UpdatePtr& act : actns.actions) {
               Entity e = {scene, entity};
                act->on_update(scene, e, dt);
            }
        }
    }

}
