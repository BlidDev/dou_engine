#include "systems/renderersys.h"
#include "components/camera.h"
#include "components/light.h"
#include "components/modelcomp.h"
#include "components/transform.h"
#include "renderer.h"
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace engine {
    struct BatchCommand {
        uint64_t sort_key;

        size_t       layer;
        unsigned int vao;
        unsigned int shader;
        unsigned int texture;

        glm::mat4 model;
        glm::mat3 normal;
        ModelComp* model_cmp;
    };

    uint64_t generate_sort_key(unsigned int layer, unsigned int vao, unsigned int shader, unsigned int texture, unsigned int misc = 0);

    void set_ubos(entt::registry& registry, RenderData& data, SceneRenderData* s_render_data, CameraComp& viewer_camera, TransformComp& viewer_trans,const glm::vec2& view_size, const bool& external_clear);

    void send_lights(entt::registry &registry, RenderData &data);
    void send_material(Material &material);

    void draw_to_camera(RenderData& data, glm::vec2 view_size, Entity& viewer,
                         entt::registry& registry, 
                         SceneRenderData* s_render_data, bool external_clear) {

      auto p_trans = viewer.get_component<TransformComp>();
      auto p_camera = viewer.get_component<CameraComp>();
      auto objects = registry.view<TransformComp, ModelComp>();
      StateCounter counter(objects.size_hint());

      set_ubos(registry, data, s_render_data, p_camera, p_trans, view_size, external_clear);

      std::vector<BatchCommand> commands;
      commands.reserve(objects.size_hint());

      for (auto [_, pos, obj] : objects.each()) {
          float distance = glm::distance(pos.position(), p_trans.position());
          if((!obj.is_immune && distance > p_camera.max_distance)) continue;

          uint64_t sort_key = generate_sort_key(obj.layer, obj.mesh.VAO, obj.material.shader, obj.material.texture);

          glm::mat4 model = pos.get_model();
          glm::mat3 normal = (obj.mesh.normals()) ? glm::transpose(glm::inverse(model)): glm::mat4(1.0f);

          commands.push_back(BatchCommand{sort_key, obj.layer, obj.mesh.VAO, obj.material.texture,obj.material.shader, model,normal,&obj});
      }

      std::sort(commands.begin(), commands.end(), [](BatchCommand& a, BatchCommand& b){
              return a.sort_key < b.sort_key;
              });

      uint64_t c_key = -1;
      size_t c_layer = -1;
      unsigned int c_shader = -1;
      unsigned int c_vao = -1;
      unsigned int c_tex = -1;

      // TODO Maybe change the global state updates to be dynamic instead of constant
      for (const BatchCommand& cmd : commands) {
          if (c_key != cmd.sort_key) {
              if (c_layer != cmd.layer) {
                  c_layer = cmd.layer;
                  LayerAtrb atrb = data.layers_atrb[c_layer];
                  if (atrb.depth) glEnable(GL_DEPTH_TEST); else glDisable(GL_DEPTH_TEST); counter.add_global();
                  glPolygonMode(GL_FRONT_AND_BACK, atrb.wireframe ? GL_LINE : GL_FILL); counter.add_global();
              }
              if (c_shader != cmd.shader) {
                  c_shader = cmd.shader;
                  glUseProgram(c_shader);
                  counter.add_shader();
              }

              if (c_tex != cmd.texture && cmd.model_cmp->material.is_textured) {
                  c_tex = cmd.texture;
                  glBindTexture(GL_TEXTURE_2D, c_tex);
                  counter.add_texture();
              }
              
              if (c_vao != cmd.vao) {
                  c_vao = cmd.vao;
                  glBindVertexArray(c_vao);
                  counter.add_vao();
              }
          }
          Shader tmp(c_shader, "");
          ModelComp* mc = cmd.model_cmp;

          set_shader_f(tmp, "dou_time", glfwGetTime());
          send_material(mc->material);

          set_shader_m4(tmp, "model", cmd.model);

          //bool filled = (obj.material.attributes & MODEL_FILLED) == MODEL_FILLED;

          if (mc->mesh.normals()) {
              glm::mat3 normal = glm::transpose(glm::inverse(cmd.model));
              set_shader_m3(mc->material.shader, "normal_mat", normal);
          }

          if (mc->mesh.nindices > 0) {
              glDrawElements(GL_TRIANGLES, mc->mesh.nindices, GL_UNSIGNED_INT, 0);
              counter.add_call();
          }
          else {
              glDrawArrays(GL_TRIANGLES, 0, mc->mesh.nvertices);
              counter.add_call();
          }

      }


      /*for (int i = 0; i < MAX_RENDER_LAYERS; i++) {
          LayerAtrb atrb = data.layers_atrb[i];


          if (!atrb.depth) {
              glDisable(GL_DEPTH_TEST);
              counter.add_global();
          }


          for (auto [_, pos, obj] : objects.each()) {
              float distance = glm::distance(pos.position(), p_trans.position());

              if((!obj.is_immune && distance > p_camera.max_distance) || obj.layer != i) continue;

              glUseProgram(obj.material.shader);
              counter.add_shader();

              send_material(obj.material);

              set_shader_f(obj.material.shader, "dou_time", glfwGetTime());
             
              glm::mat4 model = pos.get_model();
              set_shader_m4(obj.material.shader, "model", model);

              //bool filled = (obj.material.attributes & MODEL_FILLED) == MODEL_FILLED;

              if (obj.mesh.normals()) {
                  glm::mat3 normal = glm::transpose(glm::inverse(model));
                  set_shader_m3(obj.material.shader, "normal_mat", normal);
              }


              if (obj.material.is_textured) {
                  glBindTexture(GL_TEXTURE_2D, obj.material.texture);
                  counter.add_texture();
              }


              if (atrb.wireframe) {glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); counter.add_global();}
              glBindVertexArray(obj.mesh.VAO);
              counter.add_vao();


              if (obj.mesh.nindices > 0) {
                  glDrawElements(GL_TRIANGLES, obj.mesh.nindices, GL_UNSIGNED_INT, 0);
                  counter.add_call();
              }
              else {
                  glDrawArrays(GL_TRIANGLES, 0, obj.mesh.nvertices);
                  counter.add_call();
              }

              if (atrb.wireframe) {glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); counter.add_global();}
          }

          if(!atrb.depth)
          {glEnable(GL_DEPTH_TEST); counter.add_global();}
      }

      */

      glBindFramebuffer(GL_FRAMEBUFFER, 0);

      counter.summery();
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



    void present_camera(Entity& viewer, Mesh& mesh) {
        DU_ASSERT(!viewer.has_component<CameraComp>(), "Trying to present entity {} but it has no camera component", viewer.uuid());
        CameraComp& camera = viewer.get_component<CameraComp>();
        DU_ASSERT(!camera.framebuffer, "Trying to present entity {} but framebuffer is invalid", viewer.uuid());

        Shader& shader = camera.present_shader;
        DU_ASSERT(!shader, "Trying to present entity {} but shader is invalid", viewer.uuid());


        const glm::vec2 view_size = camera.framebuffer.last_scale;
        glViewport(0,0, view_size.x, view_size.y);

        glUseProgram(shader);

        glm::mat4 model_mat = glm::scale(glm::mat4(1.0f), {4.0f, 2.0f, 0.0f});
        set_shader_m4(shader, "model", model_mat);

        set_shader_f(shader,"ratio", view_size.x / view_size.y);

        glBindTexture(GL_TEXTURE_2D, camera.framebuffer.texture);


        glBindVertexArray(mesh.VAO);


        if (mesh.nindices > 0) {
          glDrawElements(GL_TRIANGLES, mesh.nindices, GL_UNSIGNED_INT, 0);
        }
        else {
          glDrawArrays(GL_TRIANGLES, 0, mesh.nvertices);
        }

    }

    void set_ubos(entt::registry& registry, RenderData& data, SceneRenderData* s_render_data, CameraComp& viewer_camera, TransformComp& viewer_trans,const glm::vec2& view_size, const bool& external_clear) {

        glm::mat4 projection = viewer_camera.get_projection(view_size);

        glm::mat4 view = viewer_camera.get_view(viewer_trans.position());

        glm::vec4 clear_color = (s_render_data) ? s_render_data->clear_color : glm::vec4(0.0f);
        glm::vec3 ambient = (s_render_data) ? s_render_data->ambient : glm::vec3(1.0f);
        float ambient_strength = (s_render_data) ? s_render_data->ambient_strength : 0.1f;


        if(!external_clear) {
            clear_buffers(clear_color, data.clear_flags);
        }

        if (viewer_camera.framebuffer.last_scale != view_size) {
            rescale_framebuffer(viewer_camera.framebuffer, view_size.x, view_size.y);
        }

        glViewport(0,0, view_size.x, view_size.y);
        glBindFramebuffer(GL_FRAMEBUFFER, viewer_camera.framebuffer);
        glClear(data.clear_flags);



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

    // Layer (8 bits) | Shader (8 bits) | Texture (16 bits) | VAO (16 bits) | Misc (16 bits)
    uint64_t generate_sort_key(unsigned int layer, unsigned int vao, unsigned int shader, unsigned int texture, unsigned int misc) {
        return 
                    (static_cast<uint64_t>(layer)<<56)   | // 8  bits
                    (static_cast<uint64_t>(shader)<<48)  | // 16 bits
                    (static_cast<uint64_t>(texture)<<32) | // 16 bits
                    (static_cast<uint64_t>(vao)<<16)     | // 8  bits
                    (static_cast<uint64_t>(misc));         // 16 bits
                
    }


} 
