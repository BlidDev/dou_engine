#include "systems/renderersys.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "components/camera.h"
#include "components/light.h"
#include "components/transform.h"
#include "components/modelcomp.h"
#include "util.h"


namespace engine {

    void send_lights(entt::registry& registry, RenderData& data);

    void opengl_renderer(RenderData& data,glm::vec2 view_size, Entity viewer, entt::registry& registry) {

        auto p_trans  = viewer.get_component<TransformComp>();
        auto p_camera = viewer.get_component<CameraComp>();
        auto objects = registry.view<TransformComp, ModelComp>();

        glm::mat4 projection = glm::perspective(glm::radians(p_camera.fovy), view_size.x / view_size.y, 0.1f, 100.0f);

        update_camera_target(p_camera, p_trans.position);
        glm::mat4 view = glm::lookAt(p_trans.position, p_camera.target, p_camera.up);

        data.bind("Matrices")
            .sub(0, sizeof(glm::mat4), glm::value_ptr(projection))
            .sub(sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view))
            .unbind();

        data.bind("Lighting")
            .sub(0, sizeof(glm::vec3), glm::value_ptr(data.ambient))
            .sub(sizeof(glm::vec3), sizeof(float), &data.ambient_strength)
            .sub(sizeof(glm::vec4), sizeof(glm::vec3), glm::value_ptr(p_trans.position))
            .unbind();

        send_lights(registry, data);

        for (auto [_, pos, obj] : objects.each()) {

            EG_ASSERT(obj.material.attributes == 0, "Model [{}] has no attributes", obj.model.name);

            if ((obj.material.attributes & MODEL_TEXTURED) == MODEL_TEXTURED)
                glBindTexture(GL_TEXTURE_2D, obj.material.texture);

            glUseProgram(obj.material.shader);

            if ((obj.material.attributes & MODEL_FILLED) == MODEL_FILLED)
                set_shader_v4(obj.material.shader, "color", obj.material.color);

            glm::mat4 model = pos.get_model(); 
            set_shader_m4(obj.material.shader, "model", model);
            
            if (obj.model.normals()) {
                glm::mat4 normal = glm::transpose(glm::inverse(model));
                set_shader_m3(obj.material.shader, "normal_mat", normal);
            }


            glBindVertexArray(obj.model.VAO);
            if (obj.model.nindices != 0) {
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                continue;
            }
            glDrawArrays(GL_TRIANGLES, 0, obj.model.nvertices);
        }


    }

    void send_lights(entt::registry& registry, RenderData& data) {
        int counter = 0;

        size_t combined_size = sizeof(glm::vec3) + sizeof(float) + sizeof(LightComp);

        auto view = registry.view<TransformComp,LightComp>();
        data.bind("SceneLights");
        data.sub(0, data.max_lights * combined_size, nullptr);

        for (auto [e, t, l]: view.each()) {
            EG_ASSERT(counter >= (int)data.max_lights, "Max number of lights [{}], exceeded", data.max_lights);
            data.sub(counter * combined_size, sizeof(glm::vec3), glm::value_ptr(t.position)) // position
                .sub(counter * combined_size + sizeof(glm::vec3), sizeof(float), nullptr) // padding
                .sub(counter * combined_size + sizeof(glm::vec4), sizeof(LightComp), &l); // light component
            counter++;
        }
        data.sub(data.max_lights * combined_size, sizeof(int), &counter);
        //if (counter < data.max_lights - 1) { // remove excess
        //    data.sub((counter + 1) * sizeof(LightComp), (data.max_lights - counter) * sizeof(LightComp), nullptr);
        //}

        data.unbind();
    }
}
