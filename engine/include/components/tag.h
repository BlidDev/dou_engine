#pragma once
#include <espch.h>

namespace engine {
    struct TagComp {
        TagComp() { tag = "";}
        TagComp(std::string tag) : tag(tag) {}
        std::string tag = "";
    };
}
