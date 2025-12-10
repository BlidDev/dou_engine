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
