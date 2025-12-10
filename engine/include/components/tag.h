#pragma once
#include <espch.h>

namespace engine {
    struct TagComp {
        TagComp() { tag = "";}
        TagComp(std::string tag) : tag(tag) {}
        std::string tag = "";
    };
}

template<>
struct fmt::formatter<engine::TagComp> : fmt::formatter<std::string>{

    auto format(const engine::TagComp tag, format_context& ctx) const {
        return fmt::formatter<std::string>::format(std::format("\"{}\"", tag.tag), ctx);
    }
};
