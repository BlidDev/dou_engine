#include "systems/renderersys.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "components/camera.h"
#include "components/transform.h"
#include "components/modelcomp.h"
#include "util.h"


namespace engine {

    void opengl_renderer(glm::vec2 view_size, Entity viewer, entt::registry& registry) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto p_trans  = viewer.get_component<TransformComp>();
        auto p_camera = viewer.get_component<CameraComp>();
        auto objects = registry.view<TransformComp, ModelComp>();


        glm::mat4 projection = glm::perspective(glm::radians(p_camera.fovy), view_size.x / view_size.y, 0.1f, 100.0f);
        update_camera_target(p_camera, p_trans.position);
        glm::vec3 forward = get_forward(p_camera.target, p_trans.position);
        glm::mat4 view = glm::lookAt(p_trans.position, p_camera.target, p_camera.up);

        EG_CORE_INFO("P: {} {} {}",p_trans.position.x, p_trans.position.y,p_trans.position.z);
        EG_CORE_INFO("T: {} {} {}",p_camera.target.x,p_camera.target.y,p_camera.target.z);
        for (auto [_, pos, obj] : objects.each()) {
            glUseProgram(obj.material.shader);
            set_shader_v4(obj.material.shader, "color", obj.material.color);
            set_shader_m4(obj.material.shader, "projection", projection);
            set_shader_m4(obj.material.shader, "view", view);
            set_shader_m4(obj.material.shader, "model", pos.get_model());
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
