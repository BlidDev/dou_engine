#include "model.h"

namespace engine {

    unsigned int apply_format(VAOType format) {
        switch (format) {
            case VAOType::BASIC: 
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
                glEnableVertexAttribArray(0);
                break;
            default: EG_CORE_ERROR("Unimplemented VAO format [{}]", (int)format); return -1;break;
        }

        return 0;

    }
    
    void thing(int a);

    Model create_model(VAOType format, float vertices[], unsigned int size, const char* name) {
        unsigned int VBO, VAO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), vertices, GL_STATIC_DRAW);

        apply_format(format);

        glBindVertexArray(0); 
        return Model{VAO, VBO, size, std::string(name)};
    }

}
