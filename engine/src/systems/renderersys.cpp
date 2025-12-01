#include "systems/renderersys.h"
#include "components/camera.h"
#include "components/light.h"
#include "components/modelcomp.h"
#include "components/transform.h"
#include "renderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace engine {

    void send_lights(entt::registry &registry, RenderData &data);
    void send_material(Material &material);

    void draw_to_camera(RenderData& data, glm::vec2 view_size, Entity& viewer,
                         entt::registry& registry, 
                         SceneRenderData* s_render_data, bool external_clear, uint32_t parent_fb) {

      auto p_trans = viewer.get_component<TransformComp>();
      auto p_camera = viewer.get_component<CameraComp>();
      auto objects = registry.view<TransformComp, ModelComp>();

      glm::mat4 projection = glm::perspective(
          glm::radians(p_camera.fovy), view_size.x / view_size.y, 0.1f, 100.0f);

      update_camera_target(p_camera, p_trans.position);
      glm::mat4 view = glm::lookAt(p_trans.position, p_camera.target, p_camera.up);

      glm::vec4 clear_color = (s_render_data) ? s_render_data->clear_color : glm::vec4(0.0f);
      glm::vec3 ambient = (s_render_data) ? s_render_data->ambient : glm::vec3(1.0f);
      float ambient_strength = (s_render_data) ? s_render_data->ambient_strength : 0.1f;


      if(!external_clear) {
          glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
          glClear(data.clear_flags);
      }

      if (p_camera.framebuffer.last_scale != view_size) {
          rescale_framebuffer(p_camera.framebuffer, view_size.x, view_size.y);
      }

      glViewport(0,0, view_size.x, view_size.y);
      glBindFramebuffer(GL_FRAMEBUFFER, p_camera.framebuffer);
      glClear(data.clear_flags);



      data.bind("Matrices")
          .sub(0, sizeof(glm::mat4), glm::value_ptr(projection))
          .sub(sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view))
          .unbind();

      data.bind("Lighting")
          .sub(0, sizeof(glm::vec3), glm::value_ptr(ambient))
          .sub(sizeof(glm::vec3), sizeof(float), &ambient_strength)
          .sub(sizeof(glm::vec4), sizeof(glm::vec3),
               glm::value_ptr(p_trans.position))
          .unbind();

      send_lights(registry, data);


      for (int i = 0; i < MAX_RENDER_LAYERS; i++) {
          LayerAtrb atrb = data.layers_atrb[i];


          if (!atrb.depth)
              glDisable(GL_DEPTH_TEST);


          for (auto [_, pos, obj] : objects.each()) {
              float distance = glm::distance(pos.position, p_trans.position);
              bool immune = (obj.material.attributes & MODEL_IMMUNE) == MODEL_IMMUNE;

              if((!immune && distance > p_camera.max_distance) || obj.layer != i) continue;

              DU_ASSERT(obj.material.attributes == 0, "Model [{}] has no attributes",
                      obj.model.name);


              glUseProgram(obj.material.shader);

              send_material(obj.material);
             
              glm::mat4 model = pos.get_model();
              set_shader_m4(obj.material.shader, "model", model);

              bool filled = (obj.material.attributes & MODEL_FILLED) == MODEL_FILLED;

              if (obj.model.normals()) {
                  glm::mat3 normal = glm::transpose(glm::inverse(model));
                  set_shader_m3(obj.material.shader, "normal_mat", normal);
              }


              if ((obj.material.attributes & MODEL_TEXTURED) == MODEL_TEXTURED)
                  glBindTexture(GL_TEXTURE_2D, obj.material.texture);


              if (atrb.wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
              glBindVertexArray(obj.model.VAO);


              if (obj.model.nindices > 0) {
                  glDrawElements(GL_TRIANGLES, obj.model.nindices, GL_UNSIGNED_INT, 0);
              }
              else {
                  glDrawArrays(GL_TRIANGLES, 0, obj.model.nvertices);
              }

              if (atrb.wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
          }

          if(!atrb.depth)
              glEnable(GL_DEPTH_TEST);
      }


      glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
                   &t.position);
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
              .sub(base + light_size, sizeof(glm::vec3), &t.position);

          s.cutoff = tmp_cutoff;
          s.outer_cutoff = tmp_outer_cutoff;
          ++counter;
        }

        data.sub(max * ssize, sizeof(int), &counter);
      }
      data.unbind();
    }

    void send_material(Material &material) {
      set_shader_v3(material.shader, "material.ambient", material.ambient);
      set_shader_v3(material.shader, "material.diffuse", material.diffuse);
      set_shader_v3(material.shader, "material.specular", material.specular);
      set_shader_f(material.shader, "material.shininess", material.shininess);
    }




    void present_camera(Entity& viewer, Model& model, uint32_t parent_fb) {
        DU_ASSERT(!viewer.has_component<CameraComp>(), "Trying to present entity {} but it has no camera component", viewer.uuid());
        CameraComp& camera = viewer.get_component<CameraComp>();
        DU_ASSERT(!camera.framebuffer, "Trying to present entity {} but framebuffer is invalid", viewer.uuid());

        Shader& shader = camera.present_shader;
        DU_ASSERT(!shader, "Trying to present entity {} but shader is invalid", viewer.uuid());




        glBindFramebuffer(GL_FRAMEBUFFER, parent_fb);

        const glm::vec2 view_size = camera.framebuffer.last_scale;
        glViewport(0,0, view_size.x, view_size.y);

        glUseProgram(shader);

        glm::mat4 model_mat = glm::scale(glm::mat4(1.0f), {4.0f, 2.0f, 0.0f});
        set_shader_m4(shader, "model", model_mat);

        set_shader_f(shader,"ratio", view_size.x / view_size.y);

        glBindTexture(GL_TEXTURE_2D, camera.framebuffer.texture);


        glBindVertexArray(model.VAO);


        if (model.nindices > 0) {
          glDrawElements(GL_TRIANGLES, model.nindices, GL_UNSIGNED_INT, 0);
        }
        else {
          glDrawArrays(GL_TRIANGLES, 0, model.nvertices);
        }

        if (parent_fb != 0) {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

    }
} 
