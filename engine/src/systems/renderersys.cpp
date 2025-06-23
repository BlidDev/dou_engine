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
        size_t dsize = sizeof(DirLightComp);
        size_t psize = sizeof(PntLightComp) + 2 * sizeof(glm::vec3); // struct + pos + padding;
        size_t allsize = data.max_lights * dsize + data.max_lights * psize + 2 * sizeof(int);
        int counter = 0, dnum = 0, pnum = 0;

        data.bind("SceneLights");
        data.sub(0, allsize, nullptr);


        auto dirs = registry.view<DirLightComp>();
        for (auto [_, d] : dirs.each()) {
            data.sub(counter * dsize, sizeof(DirLightComp), &d);
            counter++;
        }
        dnum = counter; counter = 0;

        data.sub(data.max_lights * dsize, sizeof(int), &dnum);
        size_t start = data.max_lights * dsize + sizeof(int);

        auto pnts = registry.view<TransformComp, PntLightComp>();
        for (auto [_, t, p] : pnts.each()) {
            data.sub(start + counter * psize, sizeof(glm::vec3), glm::value_ptr(t.position))
                .sub(start + counter * psize + sizeof(glm::vec3), sizeof(PntLightComp), &p);
            counter++;
        }
        pnum = counter; counter = 0;

        start = data.max_lights * dsize + data.max_lights * psize + sizeof(int);
        data.sub(start, sizeof(int), &pnum);

        data.unbind();
    }

    void send_material(Material& material) {
        set_shader_v3(material.shader, "material.ambient", material.ambient);
        set_shader_v3(material.shader, "material.diffuse", material.diffuse);
        set_shader_v3(material.shader, "material.specular", material.specular);
        set_shader_f(material.shader, "material.shininess", material.shininess);
    }
}
