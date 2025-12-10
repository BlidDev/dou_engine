#include "components/action.h"
#include "egassert.h"

namespace engine {

    void UpdateComp::serialize(YAML::Emitter& out) {
        out<<YAML::Key<<"Default"<<YAML::Value<<"Default";
    }
    void UpdateComp::dserialize(const YAML::Node& node) {
        DU_ASSERT(inner_name == "UNKNOWN","Trying to dserialize unspecified action"); 
    }

    UpdateComp::~UpdateComp() {

    }

    std::unordered_map<std::string, ActionsComp::UpdatePtr>ActionsComp::registered_actions = {};

    ActionsComp::ActionsComp() : actions() {
    }

    ActionsComp& ActionsComp::add(UpdateComp* comp, std::string name) {
        comp->inner_name = name;
        actions.push_back(UpdatePtr(comp));
        return *this;
    }

    ActionsComp& ActionsComp::add(const char* action) {
        const auto& it = registered_actions.find(std::string(action));
        DU_ASSERT(it == registered_actions.end(), "Trying to add unregistered action [{}]", action);
        actions.push_back(it->second);

        return *this;
    }

    UpdateComp* ActionsComp::get_last() {
        DU_ASSERT(actions.empty(), "Trying to get non existant action in get_last()");
        return actions.back().get();
    }

    bool ActionsComp::register_action(const std::string &name, UpdateComp *comp) {
        if (registered_actions.contains(name)) {
            DU_CORE_ERROR("Action %s is already registered", name.c_str());
            return false;
        }
        comp->inner_name = name;
        registered_actions.insert(std::make_pair(name, comp));
        return true;
    }

}
