#pragma once
#include <espch.h>

namespace engine {
    struct ShaderReturn {
        std::string vertex_code;
        std::string fragment_code;
    };

    ShaderReturn parse_shader_file(const char* path);

    struct Shader{ 
        uint32_t program;
        std::string path;

        Shader(uint32_t program, const char* path) {
            this->program = program;
            this->path = path;
        }

        Shader(uint32_t program, std::string path) {
            this->program = program;
            this->path = path;
        }

        Shader() {
            program = 0;
            path = "UNSET";
        }

        operator uint32_t() const {
            return program;
        }

        operator bool() const {
            return program != 0; 
        }

        // Frees the shader program from OpenGL memory, makes the object useless
        void free() {
            glDeleteProgram(program);
            DU_CORE_DEBUG_TRACE("Freed {}", path);
            program = 0; path = "";
        }
    };
    Shader complie_shader_file(const char* path);
    Shader complie_shader_code(const ShaderReturn& source);

    void set_shader_b(const Shader& shader, const char* name,  const bool& value);
    void set_shader_i(const Shader& shader, const char* name,  const int& value);
    void set_shader_f(const Shader& shader, const char* name,  const float& value);
    void set_shader_v2(const Shader& shader, const char* name, const glm::vec2& value);
    void set_shader_v3(const Shader& shader, const char* name, const glm::vec3& value);
    void set_shader_v4(const Shader& shader, const char* name, const glm::vec4& value);
    void set_shader_m3(const Shader& shader, const char* name, const glm::mat3& value);
    void set_shader_m4(const Shader& shader, const char* name, const glm::mat4& value);
}
