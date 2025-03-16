#pragma once
#include <string>

namespace engine {
    enum VAOType {
        BASIC = 0,
        TEXTURE,
        LIGHTING,
        LIGHTING_TEX
    };
    struct Model {
        unsigned int VAO = 0, VBO = 0, nvertices;
        std::string name = "UNKNOWN";
    };

    unsigned int apply_format(VAOType format);
    Model create_model(VAOType format, float vertices[], unsigned int size, const char* name);
}
