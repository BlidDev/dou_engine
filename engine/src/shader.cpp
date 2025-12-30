#include "shader.h"

namespace engine {
    enum ReadIndex {
        None = -1,
        Vertex, Fragment
    };
    ShaderReturn parse_shader_file(const char* path) {
        ReadIndex index = None;
        std::stringstream streams[2];

        std::ifstream file(path);
        DU_ASSERT(file.fail(), "Could not open file [{}]", path);

        std::string line = "";
        while (std::getline(file, line)) {
            if (line.empty()) { continue; }
            else if (line == "@VERTEX") {
                index = Vertex; continue;
            } else if (line == "@FRAGMENT") {
                index = Fragment; continue;
            }
            streams[index]<<line<<'\n';
        }
        file.close();
        std::string  v = streams[Vertex].str();
        std::string  f = streams[Fragment].str();

        return ShaderReturn {v,f};
    }

    Shader complie_shader_file(const char* path) {
        ShaderReturn source = parse_shader_file(path);
        const char* v = source.vertex_code.c_str();
        const char* f = source.fragment_code.c_str();

        unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &v, NULL);
        glCompileShader(vertex);

        int success;
        char v_log[512];
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertex, 512, NULL, v_log);
            DU_CORE_ERROR("Could not compile vertex shader [{}]: {}", path, v_log);
            return {0, "ERROR"};
        }

        unsigned int frag = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(frag, 1, &f, NULL);
        glCompileShader(frag);

        char f_log[512];
        glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(frag, 512, NULL, f_log);
            DU_CORE_ERROR("Could not compile fragment shader [{}]: {}", path, f_log);
            return {0, "ERROR"};
;
        }


        unsigned int program = glCreateProgram();
        glAttachShader(program, vertex);
        glAttachShader(program, frag);
        glLinkProgram(program);

        // check for linking errors
        char p_log[512];
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(frag, 512, NULL, p_log);
            DU_CORE_ERROR("Could not link shader [{}]: {}", path, p_log);
            return {0, "ERROR"};
        }
        glDeleteShader(vertex);
        glDeleteShader(frag);


        return {program, std::filesystem::path(path).filename()};
    }

    void set_shader_f(const Shader& shader, const char *name, const float& value) {
        glUniform1f(glGetUniformLocation(shader, name), value);
    }

    void set_shader_v2(const Shader& shader, const char *name, const glm::vec2& value) {
        glUniform2f(glGetUniformLocation(shader, name), value.x, value.y);
    }

    void set_shader_v3(const Shader& shader, const char *name, const glm::vec3& value) {
        glUniform3f(glGetUniformLocation(shader, name), value.x, value.y, value.z);
    }

    void set_shader_v4(const Shader& shader, const char *name, const glm::vec4& value) {
        glUniform4f(glGetUniformLocation(shader, name), value.x, value.y, value.z, value.w);
    }

    void set_shader_m3(const Shader& shader, const char *name, const glm::mat3& value) {
        glUniformMatrix3fv(glGetUniformLocation(shader, name), 1, GL_FALSE, &value[0][0]);
    }

    void set_shader_m4(const Shader& shader, const char *name, const glm::mat4& value) {
        glUniformMatrix4fv(glGetUniformLocation(shader, name), 1, GL_FALSE, &value[0][0]);
    }

}
