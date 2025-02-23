#include "luawrapper.h"
#include "component.h"
#include "scene.h"
#include "manager.h"
#include "util.h"
#include <espch.h>
#include "spdlog/fmt/bundled/args.h"
#include "spdlog/fmt/bundled/core.h"


namespace engine {
    std::string variadic_args_to_str(std::string& format, sol::variadic_args& args);
    void log_trace(std::string format, sol::variadic_args args);
    void log_info (std::string format, sol::variadic_args args);
    void log_warn (std::string format, sol::variadic_args args);
    void log_error(std::string format, sol::variadic_args args);

    void bind_vectors(sol::state& env);

    void expose_env(sol::state& env){
        expose_env_types(env);
        expose_env_functions(env);
    }


    void expose_env_functions(sol::state& env) {
        env.set_function("get_tag", get_uuid_component<TagComp>);
        env.set_function("get_transfrom", get_uuid_component<TransformComp>);
        env.set_function("get_action", get_uuid_component<ActionsComp>);
        //env.set_function("get_text", get_uuid_component<TextComp>);
        env.set_function("get_physicbody", get_uuid_component<PhysicsBodyComp>);
        env.set_function("get_camera", get_uuid_component<Camera>);
        env.set_function("get_script", get_uuid_component<LuaActionComp>);

        //env.set_function("color_from_hsv", ColorFromHSV);
        //env.set_function("get_fps", GetFPS);
        //env.set_function("get_time", GetTime);
        //env.set_function("get_key_pressed", GetKeyPressed);
        //env.set_function("is_key_down", IsKeyDown);
        //env.set_function("get_mouse_delta", GetMouseDelta);
        env.set_function("handle_mouse_delta", handle_mouse_delta);
        env.set_function("get_forward", get_forward);
        env.set_function("get_right", get_right);



        env.set_function("log_trace", log_trace);
        env.set_function("log_info",  log_info);
        env.set_function("log_warn",  log_warn);
        env.set_function("log_error", log_error);
    }


    void expose_env_types(sol::state& env) {

        auto manager = env.new_usertype<SceneManager>("SceneManager");
        manager["set_current"] = &SceneManager::set_current;
        auto scene = env.new_usertype<Scene>("Scene");
        scene["uuid_to_entt"] = &Scene::uuid_to_entt;
        scene["manager"] = &Scene::manager;

        auto uuid = env.new_usertype<UUID>("UUID",
                 sol::constructors<UUID(), UUID(uint64_t)>());
        uuid["uuid"] = sol::property(&UUID::get_uuid);
        uuid["display"] = &UUID::display;

        bind_vectors(env);

        auto cl = env.new_usertype<glm::vec4>("Color");
        cl["r"] = &glm::vec4::r;
        cl["g"] = &glm::vec4::g;
        cl["b"] = &glm::vec4::b;
        cl["a"] = &glm::vec4::a;

        auto tg = env.new_usertype<TagComp> ("Tag",
                sol::constructors<TagComp(), TagComp(std::string)>());
        tg["tag"] = &TagComp::tag;

        auto tr = env.new_usertype<TransformComp>("Transform",
                sol::constructors<TransformComp(glm::vec3,glm::vec3)>());
        tr["position"] = &TransformComp::position;
        tr["size"] = &TransformComp::size;

        //auto tx = env.new_usertype<TextComp>("Text",
        //        sol::constructors<TextComp(), TextComp(std::string, int, Color)>());
        //tx["body"] = &TextComp::body;
        //tx["font_size"] = &TextComp::font_size;
        //tx["color"] = &TextComp::color;

        auto ph = env.new_usertype<PhysicsBodyComp>("PhysicsBody",
                sol::constructors<PhysicsBodyComp()>());

        ph["gravity"] = &PhysicsBodyComp::gravity;
        ph["velocity"] = &PhysicsBodyComp::velocity;
        ph["acceleration"] = &PhysicsBodyComp::acceleration;
        ph["is_solid"] = &PhysicsBodyComp::is_solid;
        ph["is_static"] = &PhysicsBodyComp::is_static;
        ph["move_delta"] = &PhysicsBodyComp::move_delta;

        auto phb = env.new_usertype<PhysicsBodyBuilder>("PhysicsBodyBuilder",
                sol::constructors<PhysicsBodyBuilder()>());

        phb["gravity"] =      &PhysicsBodyBuilder::gravity;
        phb["velocity"] =     &PhysicsBodyBuilder::velocity;
        phb["acceleration"] = &PhysicsBodyBuilder::acceleration;
        phb["is_solid"] =     &PhysicsBodyBuilder::is_solid;
        phb["is_static"] =    &PhysicsBodyBuilder::is_static;
        phb["build"] =        &PhysicsBodyBuilder::build;

        auto cmt = env.new_usertype<Camera>("Camera");
        cmt["target"] = &Camera::target;
        cmt["up"] = &Camera::up;
        cmt["fovy"] = &Camera::fovy;
        cmt["projection"] = &Camera::projection;

        auto cm = env.new_usertype<CameraBuilder>("CameraBuilder",
                sol::constructors<CameraBuilder()>());

        cm["traget"] =     &CameraBuilder::target;
        cm["up"]     =     &CameraBuilder::up;
        cm["fovy"] =       &CameraBuilder::fovy;
        cm["projection"] = &CameraBuilder::projection;
        cm["build"] =      &CameraBuilder::build;

        env.new_enum("Shape", 
                "CAMERA_PERSPECTIVE", CameraProjection::Perspective,
                "CAMERA_ORTHOGRAPHIC", CameraProjection::Orthographic);

    }

    std::string variadic_args_to_str(std::string& format, sol::variadic_args& args) {
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
        EG_TRACE(variadic_args_to_str(format,args));
    }

    void log_info(std::string format, sol::variadic_args args) {
        EG_INFO(variadic_args_to_str(format,args));
    }

    void log_warn(std::string format, sol::variadic_args args) {
        EG_WARN(variadic_args_to_str(format,args));
    }

    void log_error(std::string format, sol::variadic_args args) {
        EG_ERROR(variadic_args_to_str(format,args));
    }


    void bind_vectors(sol::state& env) {

        auto v2 = env.new_usertype<glm::vec2>( "glm::vec2", 
                sol::constructors<glm::vec2(float, float),glm::vec2()>());
        v2["x"] = &glm::vec2::x;
        v2["y"] = &glm::vec2::y;
        
        auto v3 = env.new_usertype<glm::vec3>( "glm::vec3", 
                sol::constructors<glm::vec3(float, float, float),glm::vec3()>());
        v3[sol::meta_function::addition] = [](const glm::vec3* l, const glm::vec3* r) {
            return *l + *r;
        };

        v3[sol::meta_function::subtraction] = [](const glm::vec3* l, const glm::vec3* r) {
            return *l - *r;
        };

        v3[sol::meta_function::multiplication] = [](const glm::vec3* l, const float& r) {
            return *l * r;
        };

        v3["x"] = &glm::vec3::x;
        v3["y"] = &glm::vec3::y;
        v3["z"] = &glm::vec3::z;
    }
}
