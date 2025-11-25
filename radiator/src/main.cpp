#include <epch.h>
#include "editors.h"
#include "entry_point.h"
#include "greeter.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

int engine::on_start(engine::SceneManager* manager) {
    uint32_t screen_w = 1280, screen_h = 720;

    manager->main_window = Window("Radiator", screen_w, screen_h);
    manager->render_data.screen_w = screen_w;
    manager->render_data.screen_h = screen_h;
    engine::set_input_window(manager->main_window);

    Greeter* greeter = new Greeter;
    greeter->editor = new EScene;
    manager->register_scene_raw("EDITOREditor", greeter->editor);
    manager->register_scene_raw("EDITORGreeter", greeter);

    manager->set_current("EDITORGreeter");

    return 0;
}


std::unique_ptr<Scene> engine::create_runtime_scene() {
    return std::make_unique<RTScene>();
}

int engine::on_end(engine::SceneManager* manager) {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(manager->main_window);
    glfwTerminate();
    return 0;
}
