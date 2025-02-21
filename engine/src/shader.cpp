#include "shader.h"
#include <cstddef>
#include <fstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace engine {
    enum ReadIndex {
        None = -1,
        Vertex, Fragment
    };
    ShaderReturn parse_shader_file(const char* path) {
        ReadIndex index = None;
        std::stringstream streams[2];

        std::ifstream file(path);
        EG_ASSERT(!file.is_open(), "Could not open file [{}]", path);

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
        std::cout<<v<<'\n'<<f;

        return ShaderReturn {v,f};
    }

    int complie_shader_file(const char* path) {
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
            EG_CORE_ERROR("Could not compile vertex shader [{}]: {}", path, v_log);
            return -1;
        }

        unsigned int frag = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(frag, 1, &f, NULL);
        glCompileShader(frag);

        char f_log[512];
        glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(frag, 512, NULL, f_log);
            EG_CORE_ERROR("Could not compile fragment shader [{}]: {}", path, f_log);
            return -1;
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
            EG_CORE_ERROR("Could not link shader [{}]: {}", path, p_log);
            return -1;
        }
        glDeleteShader(vertex);
        glDeleteShader(frag);


        return program;
    }
}
