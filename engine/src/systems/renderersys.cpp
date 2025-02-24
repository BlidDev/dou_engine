#include "systems/renderersys.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "components/transform.h"
#include "components/modelcomp.h"


namespace engine {

    void opengl_renderer(entt::registry& registry) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT );

        //Camera& p_camera = player.get_component<Camera>();
        auto objects = registry.view<TransformComp, ModelComp>();

        for (auto [_, pos, obj] : objects.each()) {
            glUseProgram(obj.material.shader);
            set_shader_v4(obj.material.shader, "color", obj.material.color);
            glBindVertexArray(obj.model.VAO);
            if ((obj.material.attributes & MODEL_FILLED) == MODEL_FILLED)
                glDrawArrays(GL_TRIANGLES, 0, 3);
            if((obj.material.attributes & MODEL_WIREFRAME) == MODEL_WIREFRAME) {
                        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                        glDrawArrays(GL_TRIANGLES, 0,3);
                    }
            }

    }
}
