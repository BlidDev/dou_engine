#pragma once
#include "scene.h"

namespace engine {
    struct UpdateComp {
        UpdateComp() { inner_name = "UNKNOWN"; }
        virtual void on_update(Scene* scene, Entity self, float dt) = 0;
        virtual UpdateComp* copy() = 0;
        virtual void serialize(YAML::Emitter& out) {
            out<<YAML::Key<<"Default"<<YAML::Value<<"Default";
        }
        virtual void dserialize(const YAML::Node& node) {
            if (inner_name == "UNKNOWN")
                assert("ERROR: Trying to dserialize unspecified action"); 
        }
        virtual ~UpdateComp() {}

        std::string inner_name = "UNKNOWN";
    };

    struct ActionsComp {
        ActionsComp();
        ActionsComp (std::vector<UpdateComp*> actions) {this->actions = actions;}

        ActionsComp& add(UpdateComp* comp, std::string name = "UNKNOWN");
        ActionsComp& add(const char* action);
        UpdateComp* get_last();
        std::vector<UpdateComp*> actions;

        static bool register_action(const std::string& name, UpdateComp* comp);
        static void free_registered_actions();
    private:
        static std::unordered_map<std::string, UpdateComp*>registered_actions;
    };
}
