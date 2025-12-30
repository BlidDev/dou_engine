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
    struct Mesh {
        unsigned int VAO = 0, VBO = 0, EBO = 0, nvertices, nindices;
        std::string name = "UNKNOWN";
        int vao_format;

        bool textured();
        bool normals();

        // Frees the VAO, VBO (and if exists, EBO) handlers from OpenGL memory, makes the object useless 
        void free();
    };

    struct MeshBuilder {
        MeshBuilder (std::string name = "UNKNOWN");
        MeshBuilder& vertices(float vertices[], unsigned int size);
        MeshBuilder& textured();
        MeshBuilder& normals();
        MeshBuilder& indices(unsigned int indices[], unsigned int size);
        Mesh build();

        operator Mesh() {
            return build();
        }

    private: 
        float* vertices_p;
        unsigned int* indices_p;
        int vao_format; 

        Mesh mesh;

    };

    unsigned int apply_format(int format);

    Mesh mesh_from_file(const char* path, std::string* name = nullptr);
}
