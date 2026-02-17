#include "default_rt.h"
#include "systems/actionsys.h"
#include "systems/luasys.h"
#include "input.h"
#include "components/camera.h"
#include "systems/physicsys.h"
#include "systems/renderersys.h"

namespace engine {

    DefaultRT::DefaultRT() : Scene("DefaultRT"){
        close = false;
    }

    void DefaultRT::on_create() {
        if (!file_path.empty() && uuids.empty()) 
            add_from_file(file_path.c_str());

        if (main_camera != 0)
            viewer = uuid_to_entity(main_camera);
        else
            DU_CORE_WARN("Main camera not set for scene {}", name);
        lua_action_init(this);
        actions_init(this);
    }

    void DefaultRT::on_update(float dt) {
        if (!manager->main_window) {
            close = true;
            DU_CORE_WARN("Dou defualt RT scenes are meant to be run with a window");
            return;
        }

        if (main_camera == 0) {
            clear_buffers(s_render_data.clear_color, GL_COLOR_BUFFER_BIT);
        }

        close = is_key_pressed(GLFW_KEY_ESCAPE);

        lua_action_update(this, dt);
        actions_update(this, dt);
        fixed_physics(this, 50, dt);

        if (main_camera != 0) {
            glm::vec2 view_size = manager->main_window.size();
            rescale_camera_to_window(viewer.get_component<CameraComp>(), manager->main_window);
            draw_to_camera(manager->render_data, view_size, viewer, registry, &s_render_data);
            present_camera(viewer, get_mesh("DefaultDisplayQuad"));
        }

        manager->main_window.swap_and_poll();
    }

    void DefaultRT::on_end() {
        lua_action_end(this);
        actions_end(this);
    }

    bool DefaultRT::should_close() {
        return close || glfwWindowShouldClose(manager->main_window);
    }
} 


