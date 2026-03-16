#include "light.h"
#include <engine.h>

LightScene::LightScene() : Scene("light") { close = false; }

using namespace engine;
void LightScene::on_create() {
    s_render_data.clear_color = { 0.05f, 0.05f, 0.05f, 1.0f };
    glfwSetInputMode(manager->main_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    player = create_entity();
    main_camera = player.uuid();

    auto size = manager->main_window.size();
    player.add_component<TransformComp>(
      engine::TransformBuilder().position({0.0f, 2.0f, -2.0f}));

    player.add_component<CameraComp>(
      CameraBuilder().fovy(70.0f).pitch(0.0f).yaw(0.0f)
      .present_shader(get_shader("camerapresent.glsl"))
      .framebuffer_size(size.x, size.y).build());

    player.add_component<PhysicsBodyComp>(
      PhysicsBodyBuilder().is_solid(true).is_static(true).gravity(9.8f));

    player.add_component<ActionsComp>().add("SimpleAct");


    //==================================================

    Entity cube = create_entity();
    cube.add_component<TransformComp>(
      engine::TransformBuilder().position({0.0f, 1.0f, 0.0f}));
    cube.add_component<ModelComp>(
        get_mesh("cube"),
        MaterialBuilder()
        .set_texture(get_texture("proto.png"))
        .set_tex_repeat({2.0f,2.0f})
          .set_shader(get_shader("basic.glsl")));
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
    close = engine::is_key_pressed(GLFW_KEY_ESCAPE);

    engine::actions_update(this, dt);
    engine::fixed_physics(this, 50, dt);

    glm::vec2 view_size = manager->main_window.size();

    engine::rescale_camera_to_window(player.get_component<engine::CameraComp>(), manager->main_window);
    engine::draw_to_camera(manager->render_data, view_size, player, registry, &s_render_data);
    engine::present_camera(player, get_mesh("DefaultDisplayQuad"));

    manager->main_window.swap_and_poll();
}

void LightScene::on_end() { 
    actions_end(this);
    DU_TRACE("light end called"); 
}

bool LightScene::should_close() {
  return glfwWindowShouldClose(manager->main_window) || close;
}
