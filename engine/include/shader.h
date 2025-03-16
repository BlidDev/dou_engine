#pragma once
#include <glm/glm.hpp>
#include <string>

namespace engine {
    struct ShaderReturn {
        std::string vertex_code;
        std::string fragment_code;
    };

    ShaderReturn parse_shader_file(const char* path);

    struct Shader{ 
        uint32_t program;
        std::string path;

        operator uint32_t() {
            return program;
        }
    };
    Shader complie_shader_file(const char* path);

    void set_shader_v3(Shader shader, const char* name, glm::vec3 value);
    void set_shader_v4(Shader shader, const char* name, glm::vec4 value);
    void set_shader_m4(Shader shader, const char* name, glm::mat4 value);
}
