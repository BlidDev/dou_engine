#pragma once
#include <espch.h>

namespace engine {
    enum VAO {
        NONE    = 0b0000,
        BASIC   = 0b0001,
        INDICES = 0b0010,
        TEXTURE = 0b0100,
        NORMAL  = 0b1000
    };
    struct Model {
        unsigned int VAO = 0, VBO = 0, EBO = 0, nvertices, nindices;
        std::string name = "UNKNOWN";
        int vao_format;

        bool textured();
        bool normals();
    };

    struct ModelBuilder {
        ModelBuilder (std::string name = "UNKNOWN");
        ModelBuilder& vertices(float vertices[], unsigned int size);
        ModelBuilder& textured();
        ModelBuilder& normals();
        ModelBuilder& indices(unsigned int indices[], unsigned int size);
        Model build();

        operator Model() {
            return build();
        }

    private: 
        float* vertices_p;
        unsigned int* indices_p;
        int vao_format; 

        Model model;

    };

    unsigned int apply_format(int format);

    Model model_from_file(const char* path, std::string* name = nullptr);
}
