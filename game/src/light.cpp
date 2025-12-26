#include "light.h"
#include <engine.h>

LightScene::LightScene() : Scene("light") { close = false; }

using namespace engine;
void LightScene::on_create() {
    glfwSetInputMode(manager->main_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    player = create_entity();
    main_camera = player.uuid();

    auto size = manager->main_window.size();
    auto &t = player.add_component<TransformComp>(
      engine::TransformBuilder().position({0.0f, 2.0f, -2.0f}));

    auto &c = player.add_component<CameraComp>(
      CameraBuilder().fovy(45.0f).target({0.0, 0.0, 0.0})
      .present_shader(get_shader("camerapresent.glsl"))
      .framebuffer_size(size.x, size.y).build());
    c.last_pos = t.position();

    player.add_component<PhysicsBodyComp>(
      PhysicsBodyBuilder().is_solid(true).is_static(true).gravity(0.2f));

    player.add_component<ActionsComp>().add("SimpleAct");

    //==================================================

    Entity cube = create_entity();
    cube.add_component<TransformComp>(
      engine::TransformBuilder().position({0.0f, 1.0f, 0.0f}));
    cube.add_component<ModelComp>(
      get_mesh("cube_tex"),
      MaterialBuilder()
          .set_texture(get_texture("proto.png"))
          .set_shader(get_shader("textured.glsl")));
    cube.add_component<PhysicsBodyComp>(
      PhysicsBodyBuilder().is_solid(true).is_static(true));

    Entity plane = create_entity();
    plane.add_component<TransformComp>(TransformBuilder()
                                         .position({0.0f, -0.5f, 0.0f})
                                         .size({1000.0f, 0.0f, 1000.0f}));
    plane.add_component<PhysicsBodyComp>(
      PhysicsBodyBuilder().is_solid(true).is_static(true));
    player.add_component<SptLightComp>();

    manager->write_scene_to_file("res/light.scene", this);
    actions_init(this);
}
void LightScene::on_update(float dt) {
    close = is_key_pressed(GLFW_KEY_ESCAPE);


    actions_update(this, dt);
    if(physics(this, dt)) return;
    glm::vec2 view = manager->main_window.size();

    rescale_camera_to_window(player.get_component<CameraComp>(), manager->main_window);
    draw_to_camera(manager->render_data, view, player, registry, &s_render_data);
    present_camera(player, get_mesh("quad_tex"));
    glfwSwapBuffers(manager->main_window);
    glfwPollEvents();
}

void LightScene::on_end() { 
    actions_end(this);
    DU_TRACE("light end called"); 
}

bool LightScene::should_close() {
  return glfwWindowShouldClose(manager->main_window) || close;
}
