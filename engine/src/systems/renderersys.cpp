#include "systems/renderersys.h"
#include <glm/gtc/matrix_transform.hpp>
#include "components/camera.h"
#include "components/transform.h"
#include "components/modelcomp.h"


namespace engine {


    void opengl_renderer(glm::vec2 view_size, Entity viewer, entt::registry& registry) {

        auto p_trans  = viewer.get_component<TransformComp>();
        auto p_camera = viewer.get_component<CameraComp>();
        auto objects = registry.view<TransformComp, ModelComp>();

        glm::mat4 projection = glm::perspective(glm::radians(p_camera.fovy), view_size.x / view_size.y, 0.1f, 100.0f);

        update_camera_target(p_camera, p_trans.position);
        glm::mat4 view = glm::lookAt(p_trans.position, p_camera.target, p_camera.up);

        for (auto [_, pos, obj] : objects.each()) {

            if ((obj.material.attributes & MODEL_TEXTURED) == MODEL_TEXTURED)
                glBindTexture(GL_TEXTURE_2D, obj.material.texture);

            glUseProgram(obj.material.shader);

            if ((obj.material.attributes & MODEL_FILLED) == MODEL_FILLED)
                set_shader_v4(obj.material.shader, "color", obj.material.color);
            set_shader_m4(obj.material.shader, "projection", projection);
            set_shader_m4(obj.material.shader, "view", view);
            set_shader_m4(obj.material.shader, "model", pos.get_model());


            glBindVertexArray(obj.model.VAO);
            glDrawArrays(GL_TRIANGLES, 0, obj.model.nvertices);
        }


    }
}
