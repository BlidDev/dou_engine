#include <espch.h>

namespace engine {
    struct ShaderReturn {
        std::string vertex_code;
        std::string fragment_code;
    };

    ShaderReturn parse_shader_file(const char* path);
    int complie_shader_file(const char* path);
}
