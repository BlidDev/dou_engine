#include "model.h"

namespace engine {

    unsigned int apply_forwat(VAO format) {
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

        if (basic) {
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
        indcises_p  = nullptr;
        nvertices = -1;
        nindices = -1;

    }

    ModelBuilder& ModelBuilder::format(VAO format) {

        return *this;
    }
    ModelBuilder& ModelBuilder::vertices(float vertices[], unsigned int size) {

        return *this;
    }
    ModelBuilder& ModelBuilder::indices(int indices[], unsigned int size) {

        return *this;
    }
    Model ModelBuilder::build() {

    }




    Model create_model(VAO format, float vertices[], unsigned int size, const char* name) {
        //unsigned int VBO, VAO;
        //glGenVertexArrays(1, &VAO);
        //glGenBuffers(1, &VBO);
        //glBindVertexArray(VAO);

        //glBindBuffer(GL_ARRAY_BUFFER, VBO);
        //glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), vertices, GL_STATIC_DRAW);

        //apply_format(format);

        //if

        //glBindVertexArray(0); 
        //return Model{VAO, VBO, size, std::string(name)};
    }

}
