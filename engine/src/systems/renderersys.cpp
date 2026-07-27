#include "systems/renderersys.h"
#include "systems/rendering/uniforms.h"
#include "systems/rendering/batching.h"
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace engine {


    void draw_to_camera(RenderData& data, glm::vec2 view_size, Entity& viewer,
                         entt::registry& registry, 
                         SceneRenderData* s_render_data, bool external_clear) {

      auto p_trans = viewer.get_component<TransformComp>();
      auto p_camera = viewer.get_component<CameraComp>();
      auto objects = registry.view<TransformComp, ModelComp>();
      StateCounter counter(objects.size_hint());

      bind_and_clear_fb(p_camera.framebuffer, external_clear, s_render_data, data.clear_flags);
      set_ubos(registry, data, s_render_data, p_camera, p_trans, view_size, external_clear);

      std::vector<BatchCommand> commands;
      commands.reserve(objects.size_hint());

      for (auto [_, pos, obj] : objects.each()) {
          float distance = glm::distance(pos.position(), p_trans.position());
          if((!obj.is_immune && distance > p_camera.max_distance)) continue;

          unsigned int is_textured = obj.material.is_textured ? 1 : 0;
          uint64_t sort_key = generate_sort_key(obj.layer, obj.mesh.VAO, obj.material.shader, obj.material.texture.texture, is_textured);

          glm::mat4 model = pos.get_model();
          glm::mat3 normal = (obj.mesh.normals()) ? glm::transpose(glm::inverse(model)): glm::mat4(1.0f);

          commands.push_back(BatchCommand{sort_key, obj.layer, obj.mesh.VAO, obj.material.shader,obj.material.texture.texture, model,normal,&obj});
      }



      std::sort(commands.begin(), commands.end(), [](const BatchCommand& a, const BatchCommand& b) {
              return a.sort_key < b.sort_key;
              });

      size_t c_layer = -1;
      unsigned int c_shader = -1;
      unsigned int c_vao = -1;
      unsigned int c_tex = -1;

      // TODO Make global state changes lazy?
      for (const BatchCommand& cmd : commands) {
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
          ModelComp* mc = cmd.model_cmp;
          Shader tmp(mc->material.shader, "");

          set_shader_f(tmp, "dou_time", glfwGetTime());
          send_material(mc->material);

          set_shader_m4(tmp, "model", cmd.model);


          if (mc->mesh.normals()) {
              set_shader_m3(mc->material.shader, "normal_mat", cmd.normal);
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



      glBindFramebuffer(GL_FRAMEBUFFER, 0);

      counter.summery();
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




} 

