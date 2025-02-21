#include "model.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>


namespace engine {

    unsigned int apply_format(VAOType format) {
        switch (format) {
            case VAOType::BASIC: 
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
                glEnableVertexAttribArray(0);
                break;
            default: EG_CORE_ERROR("Unimplemented VAO format [{}]", (int)format);break;
        }

    }

    Model create_model(VAOType format, float vertices[], unsigned int size) {
        unsigned int VBO, VAO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        apply_format(format);

        glBindVertexArray(0); 
        return Model{VAO, VBO};
    }

}
