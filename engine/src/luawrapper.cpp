#include "luawrapper.h"
#include "component.h"
#include "scene.h"
#include <raylib.h>


namespace engine {
    void expose_env(sol::environment &env){
        expose_env_types(env);
        expose_env_functions(env);
    }


    void expose_env_functions(sol::environment& env) {
        env.set_function("get_uuid_transfrom", get_uuid_transfrom);
    }


    void expose_env_types(sol::environment& env) {
        auto v3 = env.new_usertype<Vector3>( "Vector3", 
                sol::constructors<Vector3(float, float, float),Vector3()>());
        v3["x"] = &Vector3::x;
        v3["y"] = &Vector3::y;
        v3["z"] = &Vector3::z;
        auto t = env.new_usertype<TransformComp>("Transform",
                sol::constructors<TransformComp(Vector3,Vector3)>());
        t["position"] = &TransformComp::position;
        t["size"] = &TransformComp::size;
    }

    TransformComp& get_uuid_transfrom(Scene* owner, UUID uuid) {
        return owner->get_uuid_component<TransformComp>(uuid);
    }
}
