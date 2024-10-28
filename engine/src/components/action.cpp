#include "components/action.h"


namespace engine {
    ActionsComp::ActionsComp() {
        actions = {};
    }

    ActionsComp& ActionsComp::add(UpdateComp* comp) {
        actions.push_back(comp);
        return *this;
    }
}
