#pragma once
#include <vector>
#include <entt.hpp>


namespace engine {
    struct UpdateComp {
        virtual void on_update(entt::registry& registry, entt::entity self, float dt) = 0;
        virtual ~UpdateComp() {}
    };

    struct ActionsComp {
        ActionsComp();
        ActionsComp (std::vector<UpdateComp*> actions) {this->actions = actions;}

        ActionsComp& add(UpdateComp* comp);
        std::vector<UpdateComp*> actions;
    };
}
