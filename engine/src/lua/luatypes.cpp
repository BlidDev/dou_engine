#include "manager.h"
#include "luawrapper.h"


namespace engine {
    void bind_vectors(sol::state& env);

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
        tr["rotation"] = &TransformComp::rotation;

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

        auto cmt = env.new_usertype<CameraComp>("Camera");
        cmt["target"] = &CameraComp::target;
        cmt["up"] = &CameraComp::up;
        cmt["fovy"] = &CameraComp::fovy;
        cmt["projection"] = &CameraComp::projection;

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


    void bind_vectors(sol::state& env) {

        auto v2 = env.new_usertype<glm::vec2>( "vec2", 
                sol::constructors<glm::vec2(float, float),glm::vec2()>());
        v2["x"] = &glm::vec2::x;
        v2["y"] = &glm::vec2::y;

        v2[sol::meta_function::addition] = [](const glm::vec2* l, const glm::vec2* r) { return *l + *r; };
        v2[sol::meta_function::subtraction] = [](const glm::vec2* l, const glm::vec2* r) { return *l - *r; };
        v2[sol::meta_function::multiplication] = [](const glm::vec2* l, const glm::vec2& r) { return *l * r; };
        v2[sol::meta_function::multiplication] = [](const glm::vec2* l, const float& r) { return *l * r; };

        
        auto v3 = env.new_usertype<glm::vec3>( "vec3", 
                sol::constructors<glm::vec3(float, float, float),glm::vec3()>());
        v3[sol::meta_function::addition] = [](const glm::vec3* l, const glm::vec3* r) { return *l + *r; };
        v3[sol::meta_function::subtraction] = [](const glm::vec3* l, const glm::vec3* r) { return *l - *r; };
        v3[sol::meta_function::multiplication] = [](const glm::vec3* l, const glm::vec3& r) { return *l * r; };
        v3[sol::meta_function::multiplication] = [](const glm::vec3* l, const float& r) { return *l * r; };

        v3["x"] = &glm::vec3::x;
        v3["y"] = &glm::vec3::y;
        v3["z"] = &glm::vec3::z;
    }
}
