#pragma once
#include <espch.h>

namespace engine {
    enum VAO {
        BASIC   = 0b0001,
        INDICES = 0b0010,
        TEXTURE = 0b0100,
        NORMAL  = 0b1000,
    };
    struct Model {
        unsigned int VAO = 0, VBO = 0, EBO = 0, nvertices, nindices;
        std::string name = "UNKNOWN";
    };

    struct ModelBuilder {
        ModelBuilder (std::string name);
        ModelBuilder& format(VAO format);
        ModelBuilder& vertices(float vertices[], unsigned int size);
        ModelBuilder& indices(int indices[], unsigned int size);
        Model build();
    private: 
        float* vertices_p;
        int* indcises_p;
        int nvertices, nindices;
        
        Model model;

    };

    unsigned int apply_format(VAO format);
    Model create_model(VAO format, float vertices[], unsigned int size, const char* name);
}
