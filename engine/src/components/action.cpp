#include "components/action.h"
#include <assert.h>
#include <ctime>
#include <stdio.h>

namespace engine {
    std::unordered_map<std::string, UpdateComp*>ActionsComp::registered_actions = {};

    ActionsComp::ActionsComp() {
        actions = {};
    }

    ActionsComp& ActionsComp::add(UpdateComp* comp, std::string name) {
        comp->inner_name = name;
        actions.push_back(comp);
        return *this;
    }

    ActionsComp& ActionsComp::add(const char* action) {
        assert(registered_actions.find(std::string(action)) != registered_actions.end() && "ERROR: Trying to add unregistered action");
        UpdateComp* tmp = registered_actions.find(action)->second;
        UpdateComp* update = tmp->copy();
        update->inner_name = tmp->inner_name;
        actions.push_back(update);

        return *this;
    }

    UpdateComp* ActionsComp::get_last() {
        assert(!actions.empty() && "ERROR: Trying to get non existant action");
        return actions.back();
    }

    bool ActionsComp::register_action(const std::string &name, UpdateComp *comp) {
        if (registered_actions.find(name) != registered_actions.end()) {
            printf("ERROR: Action %s is already registered", name.c_str());
            return false;
        }
        comp->inner_name = name;
        registered_actions.insert(std::make_pair(name, comp));
        return true;
    }

    void ActionsComp::free_registered_actions() {
        for (auto& element : registered_actions) {
            delete element.second;
        }
    }
}
