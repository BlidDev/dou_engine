#include "input.h"
#include "log.h"
#include "luawrapper.h"
#include "util.h"
#include "component.h"
#include "components/camera.h"
#include "formatting.h"

namespace engine {

    static std::string variadic_args_to_str(std::string &format, sol::variadic_args &args);
    static void log_trace(std::string format, sol::variadic_args args);
    static void log_info(std::string format, sol::variadic_args args);
    static void log_warn(std::string format, sol::variadic_args args);
    static void log_error(std::string format, sol::variadic_args args);
    static void log_critical(std::string format, sol::variadic_args args);

    static void make_child_of(Scene* owner, UUID child, UUID parent);
    static void remove_child(Scene* owner, UUID parent, UUID child);
    static void remove_children(Scene* owner, UUID parent);
    static int get_entities_relation_wrapper(Scene* owner, UUID a, UUID b);
    static bool is_ancestor_of_wrapper(Scene* owner, UUID a, UUID b);

    static void wrapper_set_wireframe(Scene* owner, size_t layer, bool flag) {
        set_layer_wireframe(owner->manager->render_data, layer, flag);
    }

    void expose_env_functions(sol::state &env) {
      env.set_function("get_uuid", get_uuid_component<UUID>);
      env.set_function("get_tag", get_uuid_component<TagComp>);
      env.set_function("get_transform", get_uuid_component<TransformComp>);
      env.set_function("get_modelcomp", get_uuid_component<ModelComp>);
      env.set_function("get_action", get_uuid_component<ActionsComp>);
      env.set_function("get_physicsbody", get_uuid_component<PhysicsBodyComp>);
      env.set_function("get_camera", get_uuid_component<CameraComp>);
      env.set_function("get_script", get_uuid_component<LuaActionComp>);
      env.set_function("get_dirlight", get_uuid_component<DirLightComp>);
      env.set_function("get_pointlight", get_uuid_component<PntLightComp>);
      env.set_function("get_spotlight", get_uuid_component<SptLightComp>);
      env.set_function("get_children", get_uuid_component<ChildrenComp>);
      env.set_function("get_parent", get_uuid_component<ParentComp>);

      env.set_function("has_uuid", has_uuid_component<UUID>);
      env.set_function("has_tag", has_uuid_component<TagComp>);
      env.set_function("has_transform", has_uuid_component<TransformComp>);
      env.set_function("has_modelcomp", has_uuid_component<ModelComp>);
      env.set_function("has_action", has_uuid_component<ActionsComp>);
      env.set_function("has_physicbody", has_uuid_component<PhysicsBodyComp>);
      env.set_function("has_camera", has_uuid_component<CameraComp>);
      env.set_function("has_script", has_uuid_component<LuaActionComp>);
      env.set_function("has_dirlight", has_uuid_component<DirLightComp>);
      env.set_function("has_pointlight", has_uuid_component<PntLightComp>);
      env.set_function("has_spotlight", has_uuid_component<SptLightComp>);
      env.set_function("has_children", has_uuid_component<ChildrenComp>);
      env.set_function("has_parent", has_uuid_component<ParentComp>);

      env.set_function("get_time", glfwGetTime);
      env.set_function("get_mouse_delta", get_mouse_delta);
      env.set_function("handle_mouse_delta", handle_mouse_delta);
      env.set_function("get_camera_forward", get_camera_forward);
      env.set_function("get_camera_right", get_camera_right);
      env.set_function("get_camera_flat_forward", get_camera_flat_forward);
      env.set_function("get_camera_dir", get_camera_dir);

      env.set_function("v3_normalize", [](glm::vec3 t){ return glm::normalize(t);});
      env.set_function("set_shader_f" , set_shader_f);
      env.set_function("set_shader_v2", set_shader_v2);
      env.set_function("set_shader_v3", set_shader_v3);
      env.set_function("set_shader_v4", set_shader_v4);


      env.set_function("is_key_down", is_key_pressed);
      env.set_function("is_key_released", is_key_released);
      env.set_function("is_key_clicked", is_key_clicked);
      env.set_function("set_input_mode", set_input_mode);


      env.set_function("log_trace", log_trace);
      env.set_function("log_info", log_info);
      env.set_function("log_warn", log_warn);
      env.set_function("log_error", log_error);
      env.set_function("log_critical", log_critical);

      env.set_function("make_child_of", make_child_of);
      env.set_function("remove_children", remove_children);
      env.set_function("remove_child", remove_child);
      env.set_function("get_entities_relation", get_entities_relation_wrapper);
      env.set_function("is_ancestor_of", is_ancestor_of_wrapper);

      env.set_function("set_layer_wireframe", wrapper_set_wireframe);

    }

    template<typename T>
    static bool try_push_arg(const sol::object& arg, fmt::dynamic_format_arg_store<fmt::format_context>& store) {
        if(arg.is<T>()) {
            store.push_back(arg.as<T>());
            return true;
        }
        return false;
    }

    static std::string variadic_args_to_str(std::string &format,
                                     sol::variadic_args &args) {
      fmt::dynamic_format_arg_store<fmt::format_context> store;
      for (const auto& a : args) {
          bool pushed = false;
          pushed |= try_push_arg<int>(a, store);
          pushed |= try_push_arg<float>(a, store);
          pushed |= try_push_arg<std::string>(a, store);
          pushed |= try_push_arg<UUID>(a, store);
          pushed |= try_push_arg<entt::entity>(a, store);
          pushed |= try_push_arg<glm::vec2>(a, store);
          pushed |= try_push_arg<glm::vec3>(a, store);
          pushed |= try_push_arg<glm::vec4>(a, store);
          if (!pushed)
              store.push_back("<UNSUPPORTED TYPE FORMAT>");
      }

      std::string str = fmt::vformat(format, store);
      return str;
    }

    static void log_trace(std::string format, sol::variadic_args args) {
      DU_TRACE(variadic_args_to_str(format, args));
    }

    static void log_info(std::string format, sol::variadic_args args) {
      DU_INFO(variadic_args_to_str(format, args));
    }

    static void log_warn(std::string format, sol::variadic_args args) {
      DU_WARN(variadic_args_to_str(format, args));
    }

    static void log_error(std::string format, sol::variadic_args args) {
      DU_ERROR(variadic_args_to_str(format, args));
    }

    static void log_critical(std::string format, sol::variadic_args args) {
      DU_CRITICAL(variadic_args_to_str(format, args));
    }


    static void make_child_of(Scene* owner, UUID child, UUID parent) {
        Entity child_e = owner->uuid_to_entity(child);
        child_e.make_child_of(parent);
    }

    static void remove_child(Scene* owner, UUID parent, UUID child) {
        Entity parent_e = owner->uuid_to_entity(parent);
        parent_e.remove_child(child);
    }

    static void remove_children(Scene* owner, UUID parent) {
        Entity parent_e = owner->uuid_to_entity(parent);
        parent_e.remove_children();
    }

    static int get_entities_relation_wrapper(Scene* owner, UUID a, UUID b) {
        return get_entities_relation(*owner, a, b);
    }

    static bool is_ancestor_of_wrapper(Scene* owner, UUID a, UUID b) {
        return is_ancestor_of(*owner, a, b);
    }
} 
