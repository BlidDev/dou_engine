#include "model.h"

namespace engine {

    unsigned int apply_format(int format) {
        unsigned int counter = 0, size = 0;
        bool basic = ((format & VAO::BASIC) == VAO::BASIC);
        bool textured = ((format & VAO::TEXTURE) == VAO::TEXTURE);
        unsigned int width = (basic ? 3 : 0) + (textured ? 2 : 0);

        if (basic) {
            glVertexAttribPointer(counter, 3, GL_FLOAT, GL_FALSE, width * sizeof(float), (void*)(size * sizeof(float)));
            glEnableVertexAttribArray(counter);
            counter++;
            size+=3;
        }

        if (textured) {
            glVertexAttribPointer(counter, 2, GL_FLOAT, GL_FALSE, width * sizeof(float), (void*)(size * sizeof(float)));
            glEnableVertexAttribArray(counter);
            counter++;
            size+=2;
        }

        return 0;

    }
    
    ModelBuilder::ModelBuilder (std::string name) {
        model.name = name;
        vertices_p = nullptr;
        indices_p  = nullptr;
        model.nvertices = 0;
        model.nindices = 0;
        vao_format = VAO::NONE;

    }

    ModelBuilder& ModelBuilder::vertices(float vertices[], unsigned int size) {
        vertices_p = vertices;
        model.nvertices = size;
        vao_format |= VAO::BASIC;
        return *this;
    }

    ModelBuilder& ModelBuilder::textured() {
        vao_format |= VAO::TEXTURE;
        return *this;
    }

    ModelBuilder& ModelBuilder::indices(unsigned int indices[], unsigned int size) {
        indices_p = indices;
        model.nindices = size;
        vao_format |= VAO::INDICES;
        return *this;
    }

    Model ModelBuilder::build() {
        glGenVertexArrays(1, &model.VAO);
        glGenBuffers(1, &model.VBO);
        if (indices_p)
            glGenBuffers(1, &model.EBO);
        EG_ASSERT(!vertices_p,"Null vertices gave to model [{}]", model.name);
        
        glBindVertexArray(model.VAO);
        glBindBuffer(GL_ARRAY_BUFFER, model.VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * model.nvertices, vertices_p, GL_STATIC_DRAW);

        if (indices_p) {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model.EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * model.nindices, indices_p, GL_STATIC_DRAW);
        }


        apply_format(vao_format);


        glBindVertexArray(0);
        EG_INFO("vao: {} vbo: {} ebo: {} nv: {} ni: {}", model.VAO, model.VBO, model.EBO, model.nvertices, model.nindices);
        return model;
    }



}
