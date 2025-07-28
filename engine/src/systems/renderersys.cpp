#include "systems/renderersys.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "components/camera.h"
#include "components/light.h"
#include "components/transform.h"
#include "components/modelcomp.h"


namespace engine {

    void send_lights(entt::registry& registry, RenderData& data);
    void send_material(Material& material);

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
                set_shader_v3(obj.material.shader, "color", obj.material.ambient);

            glm::mat4 model = pos.get_model(); 
            set_shader_m4(obj.material.shader, "model", model);
            
            if (obj.model.normals()) { // probably lighted
                glm::mat4 normal = glm::transpose(glm::inverse(model));
                set_shader_m3(obj.material.shader, "normal_mat", normal);
                send_material(obj.material);
            }


            glBindVertexArray(obj.model.VAO);
            if (obj.model.nindices != 0) {
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                continue;
            }
            glDrawArrays(GL_TRIANGLES, 0, obj.model.nvertices);
        }


    }

    struct alignas(16) TmpLight {
        glm::vec3 position;
        float padding;
        SptLightComp light;
    };

    void send_lights(entt::registry& registry, RenderData& data) {

        size_t dsize = sizeof(DirLightComp);
        size_t psize = 3 * sizeof(glm::vec4);
        size_t ssize = 5 * sizeof(glm::vec4);
        size_t max = data.max_lights;

        data.bind("DirLights");
        {
            data.sub(0, max * dsize + sizeof(int), nullptr);

            auto dirs = registry.view<DirLightComp>();
            size_t counter = 0;
            for (auto [_, d] : dirs.each()) {
                data.sub(counter * dsize, dsize, &d);
                counter++;
            }
            data.sub(max * dsize, sizeof(int), &counter);
        }
        data.unbind();

        data.bind("PntLights");
        {
            data.sub(0, max * psize + sizeof(float), nullptr);

            auto pnts = registry.view<TransformComp,PntLightComp>();
            size_t counter = 0;
            for (auto [_, t,p] : pnts.each()) {
                data.sub(counter * psize, sizeof(PntLightComp), &p)
                    .sub(counter * psize + sizeof(PntLightComp), sizeof(glm::vec3), &t.position);
                counter++;
            }
            data.sub(max * psize, sizeof(int), &counter);
        }
        data.unbind();

        size_t spt_buffer_size = ssize * max + sizeof(glm::vec4);
        data.bind("SptLights");
        {
            data.sub(0, spt_buffer_size, nullptr);

            auto spts = registry.view<TransformComp,SptLightComp>();
            size_t counter = 0, start = sizeof(glm::vec4);
            for (auto [_, t,s] : spts.each()) {
                glm::vec4 tmp = {t.position, 1.0f};
                data.sub(start + ssize * counter, sizeof(glm::vec4), &tmp)
                    .sub(start + ssize * counter + sizeof(glm::vec4), sizeof(SptLightComp),&s);
                counter++;
            }
            data.sub(0, sizeof(int), &counter);
        }
        data.unbind();
    }

    void send_material(Material& material) {
        set_shader_v3(material.shader, "material.ambient",   material.ambient);
        set_shader_v3(material.shader, "material.diffuse",   material.diffuse);
        set_shader_v3(material.shader, "material.specular",  material.specular);
        set_shader_f (material.shader, "material.shininess", material.shininess);
    }
}
