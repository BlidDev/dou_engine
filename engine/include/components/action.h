#pragma once
#include "scene.h"

namespace engine {
    struct UpdateComp {
        UpdateComp() { inner_name = "UNKNOWN"; }
        virtual void on_update(Scene* scene, Entity self, float dt) = 0;
        virtual UpdateComp* copy() = 0;
        virtual void serialize(YAML::Emitter& out);
        virtual void dserialize(const YAML::Node& node);
        virtual ~UpdateComp();

        std::string inner_name = "UNKNOWN";
    };

    struct ActionsComp {

        using UpdatePtr = std::shared_ptr<UpdateComp>;

        ActionsComp();
        ActionsComp (std::vector<UpdatePtr>& actions) {this->actions = std::move(actions);}

        ActionsComp& add(UpdateComp* comp, std::string name = "UNKNOWN");
        ActionsComp& add(const char* action);
        UpdateComp* get_last();
        std::vector<UpdatePtr> actions;

        static bool register_action(const std::string& name, UpdateComp* comp);
    private:
        static std::unordered_map<std::string, UpdatePtr>registered_actions;
    };
}
