#include "light.h"
#include <engine.h>

LightScene::LightScene() : Scene("light") { close = false; }

void LightScene::on_create() {
  set_input_window(manager->main_window);
  glfwSetInputMode(manager->main_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glEnable(GL_DEPTH_TEST);
  player = create_entity();
  main_camera = player.uuid();
  auto &t = player.add_component<TransformComp>(
      engine::TransformBuilder().position({0.0f, 2.0f, -2.0f}));
  auto &c = player.add_component<CameraComp>(
      CameraBuilder().fovy(45.0f).target({0.0, 0.0, 0.0}).build());
  c.last_pos = t.position;
  player.add_component<PhysicsBodyComp>(
      PhysicsBodyBuilder().is_solid(true).is_static(true).gravity(0.2f));
  player.add_component<ActionsComp>(ActionsComp().add("SimpleAct"));

  Entity tri = create_entity();
  tri.add_component<TransformComp>(
      engine::TransformBuilder().position({0.0f, 1.0f, 0.0f}));
  tri.add_component<ModelComp>(
      get_model("cube_tex"),
      MaterialBuilder()
          .set_texture(get_texture("res/textures/proto.png"))
          .set_shader(get_shader("res/shaders/textured.glsl")));
  tri.add_component<PhysicsBodyComp>(
      PhysicsBodyBuilder().is_solid(true).is_static(true));

  Entity plane = create_entity();
  plane.add_component<TransformComp>(TransformBuilder()
                                         .position({0.0f, -0.5f, 0.0f})
                                         .size({1000.0f, 0.0f, 1000.0f}));
  plane.add_component<PhysicsBodyComp>(
      PhysicsBodyBuilder().is_solid(true).is_static(true));

  manager->write_scene_to_file("res/light.scene", this);
}
void LightScene::on_update(float dt) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  if (is_key_pressed(GLFW_KEY_ESCAPE))
    close = true;

  actions(this, dt);
  physics(registry, dt);
  if (aabb_check(*this, dt))
    return;
  glm::vec2 view = manager->main_window.size();

  opengl_renderer(manager->render_data, view, player, registry);
  glfwSwapBuffers(manager->main_window);
  glfwPollEvents();
}

void LightScene::on_end() { DU_TRACE("light end called"); }

bool LightScene::should_close() {
  return glfwWindowShouldClose(manager->main_window) || close;
}
