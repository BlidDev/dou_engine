#include "systems/rendering/uniforms.h"

#include <glm/gtc/type_ptr.hpp>
#include "components/light.h"


namespace engine {

    void bind_and_clear_fb(Frambuffer& framebuffer, bool external_clear, SceneRenderData* s_render_data, int clear_flags) {
        glm::vec4 clear_color = (s_render_data) ? s_render_data->clear_color : glm::vec4(0.0f);
        const glm::vec2& view_size = framebuffer.last_scale;


        if(!external_clear) {
            clear_buffers(clear_color, clear_flags);
        }

        if (framebuffer.last_scale != view_size) {
            rescale_framebuffer(framebuffer, view_size.x, view_size.y);
        }

        glViewport(0,0, view_size.x, view_size.y);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glClear(clear_flags);
    }


    void set_ubos(entt::registry& registry, RenderData& data, SceneRenderData* s_render_data, CameraComp& viewer_camera, TransformComp& viewer_trans,const glm::vec2& view_size, const bool& external_clear) {

        glm::mat4 projection = viewer_camera.get_projection(view_size);
        glm::mat4 view = viewer_camera.get_view(viewer_trans.position());
        glm::vec3 ambient = (s_render_data) ? s_render_data->ambient : glm::vec3(1.0f);
        float ambient_strength = (s_render_data) ? s_render_data->ambient_strength : 0.1f;



        data.bind("Matrices")
            .sub(0, sizeof(glm::mat4), glm::value_ptr(projection))
            .sub(sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view))
            .unbind();

        data.bind("Lighting")
            .sub(0, sizeof(glm::vec3), glm::value_ptr(ambient))
            .sub(sizeof(glm::vec3), sizeof(float), &ambient_strength)
            .sub(sizeof(glm::vec4), sizeof(glm::vec3),
                    glm::value_ptr(viewer_trans.position_ref()))
            .unbind();

        send_lights(registry, data);
    }

    void send_lights(entt::registry &registry, RenderData &data) {
        size_t max = data.max_lights;

        size_t dsize = sizeof(DirLightComp);
        data.bind("DirLights");
        {
            data.sub(0, max * dsize + sizeof(int), nullptr);

            auto dirs = registry.view<DirLightComp>();
            size_t counter = 0;
            for (auto [_, d] : dirs.each()) {
                if (counter >= max)
                    break;
                data.sub(counter * dsize, dsize, &d);
                counter++;
            }
            data.sub(max * dsize, sizeof(int), &counter);
        }
        data.unbind();

        size_t psize = 3 * sizeof(glm::vec4);
        data.bind("PntLights");
        {
            data.sub(0, max * psize + sizeof(float), nullptr);

            auto pnts = registry.view<TransformComp, PntLightComp>();
            size_t counter = 0;
            for (auto [_, t, p] : pnts.each()) {
                if (counter >= max)
                    break;
                data.sub(counter * psize, sizeof(PntLightComp), &p)
                    .sub(counter * psize + sizeof(PntLightComp), sizeof(glm::vec3),
                            &t.position_ref());
                counter++;
            }
            data.sub(max * psize, sizeof(int), &counter);
        }
        data.unbind();

        size_t pos_size = sizeof(glm::vec4);
        size_t light_size = sizeof(SptLightComp);
        size_t ssize = pos_size + light_size;
        data.bind("SptLights");
        {
            data.sub(0, max * ssize + sizeof(int), nullptr);

            auto spts = registry.view<TransformComp, SptLightComp>();
            size_t counter = 0;

            for (auto [_, t, s] : spts.each()) {
                if (counter >= max)
                    break;
                float tmp_cutoff = s.cutoff;
                float tmp_outer_cutoff = s.outer_cutoff;
                s.cutoff = glm::cos(glm::radians(tmp_cutoff));
                s.outer_cutoff = glm::cos(glm::radians(tmp_outer_cutoff));

                size_t base = counter * ssize;

                data.sub(base, light_size, &s)
                    .sub(base + light_size, sizeof(glm::vec3), &t.position_ref());

                s.cutoff = tmp_cutoff;
                s.outer_cutoff = tmp_outer_cutoff;
                ++counter;
            }

            data.sub(max * ssize, sizeof(int), &counter);
        }
        data.unbind();
    }

    void send_material(Material &material) {
        set_shader_b(material.shader, "material.is_textured", material.is_textured);
        set_shader_v2(material.shader, "material.tex_repeat", material.tex_repeat);
        set_shader_v2(material.shader, "material.tex_offset", material.tex_offset);
        set_shader_v3(material.shader, "material.ambient", material.ambient);
        set_shader_v3(material.shader, "material.diffuse", material.diffuse);
        set_shader_v3(material.shader, "material.specular", material.specular);
        set_shader_f (material.shader, "material.shininess", material.shininess);
    }
}
