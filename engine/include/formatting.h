#pragma once

template<>
struct fmt::formatter<glm::vec2> : fmt::formatter<std::string>{

    auto format(const glm::vec2& v, format_context& ctx) const {
        return fmt::formatter<std::string>::format(std::format("({}, {})", v.x, v.y), ctx);
    }
};

template<>
struct fmt::formatter<glm::vec3> : fmt::formatter<std::string>{

    auto format(const glm::vec3& v, format_context& ctx) const {
        return fmt::formatter<std::string>::format(std::format("({}, {}, {})", v.x, v.y, v.z), ctx);
    }
};

template<>
struct fmt::formatter<glm::vec4> : fmt::formatter<std::string>{

    auto format(const glm::vec4& v, format_context& ctx) const {
        return fmt::formatter<std::string>::format(std::format("({}, {}, {}, {})", v.x, v.y, v.z, v.w), ctx);
    }
};

template<>
struct fmt::formatter<entt::entity> : fmt::formatter<std::string>{

    auto format(const entt::entity& v, format_context& ctx) const {
        return fmt::formatter<std::string>::format(std::format("{}",(uint32_t)v), ctx);
    }
};

template<typename T, size_t size>
struct fmt::formatter<std::array<T,size>> : fmt::formatter<std::string>{

    auto format(const std::array<entt::entity,size>& array, format_context& ctx) const {
        auto out = ctx.out();
        out = fmt::format_to(out, "[");
        for (std::size_t i = 0; i < size; ++i) {
            out = fmt::format_to(out, "{}{}", array[i], (i == size - 1 ? "" : ", "));
        }
        return fmt::format_to(out, "]");    
    }
};
