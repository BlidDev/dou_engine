#include "input.h"
#include "log.h"
#include "luawrapper.h"
#include "util.h"

namespace engine {

    std::string variadic_args_to_str(std::string &format, sol::variadic_args &args);
    void log_trace(std::string format, sol::variadic_args args);
    void log_info(std::string format, sol::variadic_args args);
    void log_warn(std::string format, sol::variadic_args args);
    void log_error(std::string format, sol::variadic_args args);

    void make_child_of(Scene* owner, UUID child, UUID parent);
    void remove_children(Scene* owner, UUID parent);

    void expose_env_functions(sol::state &env) {
      env.set_function("get_tag", get_uuid_component<TagComp>);
      env.set_function("get_transform", get_uuid_component<TransformComp>);
      env.set_function("get_action", get_uuid_component<ActionsComp>);
      env.set_function("get_physicbody", get_uuid_component<PhysicsBodyComp>);
      env.set_function("get_camera", get_uuid_component<CameraComp>);
      env.set_function("get_script", get_uuid_component<LuaActionComp>);
      env.set_function("get_spotlight", get_uuid_component<SptLightComp>);

      // env.set_function("color_from_hsv", ColorFromHSV);
      // env.set_function("get_fps", GetFPS);
      env.set_function("get_time", glfwGetTime);
      env.set_function("get_mouse_delta", get_mouse_delta);
      env.set_function("handle_mouse_delta", handle_mouse_delta);
      env.set_function("get_forward", get_forward);
      env.set_function("get_right", get_right);
      env.set_function("update_camera_target", update_camera_target);
      env.set_function("get_flat_forward", get_flat_forward);
      env.set_function("get_camera_dir", get_camera_dir);

      env.set_function("v3_normalize", [](glm::vec3 t){ return glm::normalize(t);});

      env.set_function("is_key_down", is_key_pressed);
      env.set_function("set_input_mode", set_input_mode);


      env.set_function("log_trace", log_trace);
      env.set_function("log_info", log_info);
      env.set_function("log_warn", log_warn);
      env.set_function("log_error", log_error);

      env.set_function("make_child_of", make_child_of);
      env.set_function("remove_children", remove_children);
    }

    std::string variadic_args_to_str(std::string &format,
                                     sol::variadic_args &args) {
      fmt::dynamic_format_arg_store<fmt::format_context> store;
      for (auto a : args) {
        if (a.is<int>())
          store.push_back(a.as<int>());
        else if (a.is<float>())
          store.push_back(a.as<float>());
        else if (a.is<std::string>())
          store.push_back(a.as<std::string>());
      }

      std::string str = fmt::vformat(format, store);
      return str;
    }

    void log_trace(std::string format, sol::variadic_args args) {
      EG_TRACE(variadic_args_to_str(format, args));
    }

    void log_info(std::string format, sol::variadic_args args) {
      EG_INFO(variadic_args_to_str(format, args));
    }

    void log_warn(std::string format, sol::variadic_args args) {
      EG_WARN(variadic_args_to_str(format, args));
    }

    void log_error(std::string format, sol::variadic_args args) {
      EG_ERROR(variadic_args_to_str(format, args));
    }


    void make_child_of(Scene* owner, UUID child, UUID parent) {
        Entity child_e = owner->uuid_to_entity(child);
        child_e.make_child_of(parent);
    }


    void remove_children(Scene* owner, UUID parent) {
        Entity parent_e = owner->uuid_to_entity(parent);
        parent_e.remove_children();
    }
} 
