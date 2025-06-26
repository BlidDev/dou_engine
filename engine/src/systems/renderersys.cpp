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

    void send_lights(entt::registry& registry, RenderData& data) {

        size_t dsize = sizeof(DirLightComp);      // should be 32
        size_t psize = sizeof(PntLightComp);      // should be 32
        size_t pssize = sizeof(glm::vec4);      // should be 32
        size_t max = data.max_lights;

        // Total size, padded float section aligned to 16 bytes
        size_t allsize = max * (dsize + psize + pssize);
        size_t float_offset = (allsize + 15) & ~15; // align to 16
        allsize += float_offset + 16;                // 2 floats = 8B + 8B pad

        data.bind("SceneLights");
        data.sub(0, allsize, nullptr);

        float dnum = 0, pnum = 0;
        size_t counter = 0;

        // Upload DirLights
        auto dirs = registry.view<DirLightComp>();
        for (auto [_, d] : dirs.each()) {
            data.sub(counter * dsize, dsize, &d);
            counter++;
        }
        dnum = (float)counter;

        // Upload PntLights
        counter = 0;
        size_t pstart = dsize * max;

        auto pnts = registry.view<TransformComp, PntLightComp>();
        for (auto [_, t, p] : pnts.each()) {
            data.sub(pstart + counter * psize, psize, &p);
            counter++;
        }
        pnum = (float)counter;

        size_t ps_start = max * (dsize + psize);

        size_t i = 0;
        for (auto [_, t, p] : pnts.each()) {
            if (i >= counter) break;
            data.sub(ps_start + i * pssize, pssize, &t.position);
            i++;
        }

        // Upload 2 floats (must be at 16B-aligned offset)
        data.sub(float_offset, sizeof(float), &dnum)
            .sub(float_offset + sizeof(float), sizeof(float), &pnum);

        data.unbind();
    }

    void send_material(Material& material) {
        set_shader_v3(material.shader, "material.ambient",   material.ambient);
        set_shader_v3(material.shader, "material.diffuse",   material.diffuse);
        set_shader_v3(material.shader, "material.specular",  material.specular);
        set_shader_f (material.shader, "material.shininess", material.shininess);
    }
}
