#include "thing.h"
#include "renderer.h"


ThingScene::ThingScene() : Scene("thing") {
    close = false;
}

void ThingScene::on_create() {
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glfwSwapInterval(1);
    set_input_window(manager->main_window);
    glfwSetInputMode(manager->main_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    make_default_ubos(manager);

    set_clear_color(manager->render_data, {0.0f,0.0f,0.0f,1.0f});
    set_clear_flags(manager->render_data, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    add_from_file("res/thing.scene");
    update_render_data(manager, this);

    player = uuid_to_entity(main_camera);
    lua_action_init(this);
}


void ThingScene::on_update(float dt) {

    if(is_key_pressed(GLFW_KEY_ESCAPE))
        close = true;

    actions(this,dt);
    lua_action_update(this, dt);
    physics(registry,dt);
    if (aabb_check(*this, dt)) return;
    glm::vec2 view = manager->main_window.size();

    opengl_renderer(manager->render_data,view, player, registry);
    glfwSwapBuffers(manager->main_window);
    glfwPollEvents();
}


void ThingScene::on_end() {
    EG_TRACE("thing end called");
    lua_action_end(this);
}

bool ThingScene::should_close() {
    return glfwWindowShouldClose(manager->main_window) || close ;
}
