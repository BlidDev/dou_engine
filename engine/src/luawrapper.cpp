#include "luawrapper.h"
#include "component.h"
#include "components/primitive.h"
#include "scene.h"
#include "util.h"
#include "ops.hpp"
#include <espch.h>


namespace engine {
    void log_trace(std::string format);
    void log_info(std::string format);
    void log_warn(std::string format);
    void log_error(std::string format);

    void bind_vectors(sol::state& env);

    void expose_env(sol::state& env){
        expose_env_types(env);
        expose_env_functions(env);
    }


    void expose_env_functions(sol::state& env) {
        env.set_function("get_tag", get_uuid_component<TagComp>);
        env.set_function("get_transfrom", get_uuid_component<TransformComp>);
        env.set_function("get_primitive", get_uuid_component<PrimitiveComp>);
        env.set_function("get_action", get_uuid_component<ActionsComp>);
        env.set_function("get_text", get_uuid_component<TextComp>);
        env.set_function("get_physicbody", get_uuid_component<PhysicsBodyComp>);
        env.set_function("get_camera", get_uuid_component<Camera>);
        env.set_function("get_script", get_uuid_component<LuaActionComp>);

        env.set_function("color_from_hsv", ColorFromHSV);
        env.set_function("get_fps", GetFPS);
        env.set_function("get_time", GetTime);
        env.set_function("is_key_down", IsKeyDown);
        env.set_function("get_mouse_delta", GetMouseDelta);
        env.set_function("handle_mouse_delta", handle_mouse_delta);
        env.set_function("get_forward", get_forward);
        env.set_function("get_right", get_right);



        env.set_function("log_trace", log_trace);
        env.set_function("log_info",  log_info);
        env.set_function("log_warn",  log_warn);
        env.set_function("log_error", log_error);
    }


    void expose_env_types(sol::state& env) {
        auto scene = env.new_usertype<Scene>("Scene");
        scene["uuid_to_entt"] = &Scene::uuid_to_entt;

        auto uuid = env.new_usertype<UUID>("UUID",
                 sol::constructors<UUID(), UUID(uint64_t)>());
        uuid["uuid"] = sol::property(&UUID::get_uuid);
        uuid["display"] = &UUID::display;

        bind_vectors(env);

        auto cl = env.new_usertype<Color>("Color");
        cl["r"] = &Color::r;
        cl["g"] = &Color::g;
        cl["b"] = &Color::b;
        cl["a"] = &Color::a;

        auto tg = env.new_usertype<TagComp> ("Tag",
                sol::constructors<TagComp(), TagComp(std::string)>());
        tg["tag"] = &TagComp::tag;

        auto tr = env.new_usertype<TransformComp>("Transform",
                sol::constructors<TransformComp(Vector3,Vector3)>());
        tr["position"] = &TransformComp::position;
        tr["size"] = &TransformComp::size;

        env.new_enum("Shape", 
                "PLANE", PrimitiveComp::Shape::PLANE,
                "CUBE", PrimitiveComp::Shape::CUBE,
                "SPHERE", PrimitiveComp::Shape::SPHERE);

        auto pr = env.new_usertype<PrimitiveComp>("Primitive",
                    sol::constructors<PrimitiveComp(PrimitiveComp::Shape, Color, int)>());
        pr["shape"] = &PrimitiveComp::shape;
        pr["color"] = &PrimitiveComp::color;
        pr["attributes"] = &PrimitiveComp::attributes;

        auto tx = env.new_usertype<TextComp>("Text",
                sol::constructors<TextComp(), TextComp(std::string, int, Color)>());
        tx["body"] = &TextComp::body;
        tx["font_size"] = &TextComp::font_size;
        tx["color"] = &TextComp::color;

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
        cmt["position"] = &Camera::position;
        cmt["target"] = &Camera::target;
        cmt["up"] = &Camera::up;
        cmt["fovy"] = &Camera::fovy;
        cmt["projection"] = &Camera::projection;

        auto cm = env.new_usertype<CameraBuilder>("CameraBuilder",
                sol::constructors<CameraBuilder()>());

        cm["position"] =   &CameraBuilder::position;
        cm["traget"] =     &CameraBuilder::target;
        cm["up"]     =     &CameraBuilder::up;
        cm["fovy"] =       &CameraBuilder::fovy;
        cm["projection"] = &CameraBuilder::projection;
        cm["build"] =      &CameraBuilder::build;

        env.new_enum("Shape", 
                "CAMERA_PERSPECTIVE", CameraProjection::CAMERA_PERSPECTIVE,
                "CAMERA_ORTHOGRAPHIC", CameraProjection::CAMERA_ORTHOGRAPHIC);

    }

    void log_trace(std::string format) {
        EG_TRACE(format);
    }

    void log_info(std::string format) {
        EG_INFO(format);
    }

    void log_warn(std::string format) {
        EG_WARN(format);
    }

    void log_error(std::string format) {
        EG_ERROR(format);
    }


    void bind_vectors(sol::state& env) {

        auto v2 = env.new_usertype<Vector2>( "Vector2", 
                sol::constructors<Vector2(float, float),Vector2()>());
        v2["x"] = &Vector2::x;
        v2["y"] = &Vector2::y;
        
        auto v3 = env.new_usertype<Vector3>( "Vector3", 
                sol::constructors<Vector3(float, float, float),Vector3()>());
        v3[sol::meta_function::addition] = [](const Vector3* l, const Vector3* r) {
            return *l + *r;
        };

        v3[sol::meta_function::subtraction] = [](const Vector3* l, const Vector3* r) {
            return *l - *r;
        };

        v3[sol::meta_function::multiplication] = [](const Vector3* l, const float& r) {
            return *l * r;
        };

        v3["x"] = &Vector3::x;
        v3["y"] = &Vector3::y;
        v3["z"] = &Vector3::z;
    }
}
