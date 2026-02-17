#include "manager.h"
#include "luawrapper.h"
#include "component.h"


namespace engine {
    static void bind_entity(sol::state& env);
    static void bind_glm(sol::state& env);
    static void bind_transform(sol::state& env);
    static void bind_texture_shader_mesh(sol::state& env);
    static void bind_material(sol::state& env);
    static void bind_model(sol::state& env);
    static void bind_hierarchy(sol::state& env);

    void expose_env_types(sol::state& env) {

        auto manager = env.new_usertype<SceneManager>("SceneManager");
        manager["set_current"] = &SceneManager::set_current;
        manager["set_should_close"] = &SceneManager::set_should_close;
        manager["should_close"] = &SceneManager::should_close;
        auto scene = env.new_usertype<Scene>("Scene");
        scene["uuid_to_entt"] = &Scene::uuid_to_entt;
        scene["manager"] = &Scene::manager;
        scene["get_shader"] = &Scene::get_shader;
        scene["get_mesh"] = &Scene::get_mesh;

        auto uuid = env.new_usertype<UUID>("UUID",
                 sol::constructors<UUID(), UUID(uint64_t)>());
        uuid["uuid"] = sol::property(&UUID::get_uuid);

        bind_glm(env);

        auto tg = env.new_usertype<TagComp> ("Tag",
                sol::constructors<TagComp(), TagComp(std::string)>());
        tg["tag"] = &TagComp::tag;

        bind_transform(env);
        bind_texture_shader_mesh(env);
        bind_material(env);
        bind_model(env);

        auto ph = env.new_usertype<PhysicsBodyComp>("PhysicsBody",
                sol::constructors<PhysicsBodyComp()>());

        ph["gravity"] = &PhysicsBodyComp::gravity;
        ph["velocity"] = &PhysicsBodyComp::velocity;
        ph["slipperiness"] = &PhysicsBodyComp::slipperiness;
        ph["mass"] = &PhysicsBodyComp::mass;
        ph["is_solid"] = &PhysicsBodyComp::is_solid;
        ph["is_static"] = &PhysicsBodyComp::is_static;
        ph["move_delta"] = &PhysicsBodyComp::move_delta;

        auto phb = env.new_usertype<PhysicsBodyBuilder>("PhysicsBodyBuilder",
                sol::constructors<PhysicsBodyBuilder()>());

        phb["gravity"] =      &PhysicsBodyBuilder::gravity;
        phb["velocity"] =     &PhysicsBodyBuilder::velocity;
        phb["mass"] =         &PhysicsBodyBuilder::mass;
        phb["slipperiness"] =         &PhysicsBodyBuilder::slipperiness;
        phb["is_solid"] =     &PhysicsBodyBuilder::is_solid;
        phb["is_static"] =    &PhysicsBodyBuilder::is_static;
        phb["build"] =        &PhysicsBodyBuilder::build;

        auto cmt = env.new_usertype<CameraComp>("Camera");
        cmt["pitch"] = &CameraComp::pitch;
        cmt["yaw"] = &CameraComp::yaw;
        cmt["set_pitch"] = &CameraComp::set_pitch;
        cmt["set_yaw"] = &CameraComp::set_yaw;
        cmt["up"] = &CameraComp::up;
        cmt["fovy"] = &CameraComp::fovy;
        cmt["projection"] = &CameraComp::projection_mode;

        auto cm = env.new_usertype<CameraBuilder>("CameraBuilder",
                sol::constructors<CameraBuilder()>());

        cm["pitch"] =     &CameraBuilder::pitch;
        cm["yaw"] =       &CameraBuilder::yaw;
        cm["up"]     =     &CameraBuilder::up;
        cm["fovy"] =       &CameraBuilder::fovy;
        cm["projection"] = &CameraBuilder::projection_mode;
        cm["max_distance"] =       &CameraBuilder::max_distance;
        cm["framebuffer_size"] =       &CameraBuilder::framebuffer_size;
        cm["present_shader"] =       &CameraBuilder::present_shader;
        cm["build"] =      &CameraBuilder::build;

        auto dir = env.new_usertype<DirLightComp>("DirLight",
                sol::constructors<DirLightComp()>());
        dir["direction"] =  &DirLightComp::direction;
        dir["color"] =  &DirLightComp::color;
        dir["strength"] = &DirLightComp::strength;
        dir["ambient"] =  &DirLightComp::ambient;

        auto pnt = env.new_usertype<PntLightComp>("PntLight",
                sol::constructors<PntLightComp()>());
        pnt["color"] =  &PntLightComp::color;
        pnt["constant"] = &PntLightComp::constant;
        pnt["linear"] = &PntLightComp::linear;
        pnt["quadratic"] = &PntLightComp::quadratic;

        auto spt = env.new_usertype<SptLightComp>("SptLight",
                sol::constructors<SptLightComp()>());
        spt["direction"] =  &SptLightComp::direction;
        spt["color"] =  &SptLightComp::color;
        spt["constant"] = &SptLightComp::constant;
        spt["linear"] = &SptLightComp::linear;
        spt["quadratic"] = &SptLightComp::quadratic;
        spt["cutoff"] =  &SptLightComp::cutoff;
        spt["outer_cutoff"] =  &SptLightComp::outer_cutoff;


        env.new_enum("CameraProjection", 
                "CAMERA_PERSPECTIVE", CameraProjection::Perspective,
                "CAMERA_ORTHOGRAPHIC", CameraProjection::Orthographic);

        bind_hierarchy(env);
    }

    static void bind_entity(sol::state& env) {
        auto ent = env.new_usertype<Entity>( "Entity", 
                sol::constructors<Entity(), 
                                  Entity(Scene*, entt::entity)>());
        ent["uuid"] = &Entity::uuid;
        ent["id"] = &Entity::id;
        ent["scene_ptr"] = &Entity::scene_ptr;
        ent["is_parent"] = &Entity::is_parent;
        ent["get_children"] = &Entity::get_children;
        ent["is_child"] = &Entity::is_child;
    }

    static void bind_transform(sol::state& env) {
        auto tr = env.new_usertype<TransformComp>("Transform",
                sol::constructors<TransformComp(glm::vec3,glm::vec3)>());
        tr["position"] = &TransformComp::position;
        tr["size"] = &TransformComp::size;
        tr["rotation"] = &TransformComp::rotation;

        tr["set_position"] = &TransformComp::set_position;
        tr["set_size"] =     &TransformComp::set_size;
        tr["set_rotation"] = &TransformComp::set_rotation;

        tr["translate"] = &TransformComp::translate;
        tr["translate_x"] = &TransformComp::translate_x;
        tr["translate_y"] = &TransformComp::translate_y;
        tr["translate_z"] = &TransformComp::translate_z;

        tr["scale"] =     &TransformComp::scale;
        tr["scale_x"] =     &TransformComp::scale_x;
        tr["scale_y"] =     &TransformComp::scale_y;
        tr["scale_z"] =     &TransformComp::scale_z;


        tr["rotate"] = &TransformComp::rotate;
        tr["rotate_y"] = &TransformComp::rotate_y;
        tr["rotate_x"] = &TransformComp::rotate_x;
        tr["rotate_z"] = &TransformComp::rotate_z;

        auto trb = env.new_usertype<TransformBuilder>("TransformBuilder",
                sol::constructors<TransformBuilder()>());

        trb["position"] = &TransformBuilder::position;
        trb["size"] = &TransformBuilder::size;
        trb["rotation"] = &TransformBuilder::rotation;
        trb["build"] = &TransformBuilder::build;
    }

    static void bind_texture_shader_mesh(sol::state& env) {
        auto sh = env.new_usertype<Shader>( "Shader", 
                sol::constructors<Shader(), 
                                  Shader(uint32_t, const char*),
                                  Shader(uint32_t, std::string)>());
        sh["program"] = &Shader::program;
        sh["path"]    = &Shader::path;

        auto tx = env.new_usertype<Texture>( "Texture", 
                sol::constructors<>());
        tx["texture"] = &Texture::texture;
        tx["path"]    = &Texture::path;
        tx["w"]       = &Texture::w;
        tx["h"]       = &Texture::h;
        tx["nrc"]       = &Texture::nrc;

        auto msh = env.new_usertype<Mesh>( "Mesh", 
                sol::constructors<>());

        msh["VAO"] = &Mesh::VAO;
        msh["VBO"] = &Mesh::VBO;
        msh["EBO"] = &Mesh::EBO;
        msh["nvertices"] = &Mesh::nvertices;
        msh["nindices"] = &Mesh::nindices;
        msh["name"] = &Mesh::name;
        msh["vao_format"] = &Mesh::vao_format;
        msh["textured"] = &Mesh::textured;
        msh["normals"] = &Mesh::normals;
    }

    static void bind_material(sol::state& env) {

        auto mt = env.new_usertype<Material>( "Material", 
                sol::constructors<>());

        mt["shader"]     = &Material::shader;
        mt["texture"]    = &Material::texture;
        mt["ambient"]    = &Material::shader;
        mt["diffuse"]    = &Material::shader;
        mt["specular"]   = &Material::specular;
        mt["shininess"]  = &Material::shininess;
        mt["attributes"] = &Material::attributes;
        mt["print"]      = &Material::print;

        auto mtb = env.new_usertype<MaterialBuilder>( "MaterialBuilder", 
                sol::constructors<MaterialBuilder()>());

        mtb["set_shader"]     = &MaterialBuilder::set_shader;
        mtb["set_texture"]    = &MaterialBuilder::set_texture;
        mtb["set_ambient"]    = &MaterialBuilder::set_shader;
        mtb["set_diffuse"]    = &MaterialBuilder::set_shader;
        mtb["set_specular"]   = &MaterialBuilder::set_specular;
        mtb["set_shininess"]  = &MaterialBuilder::set_shininess;
        mtb["set_attributes"] = &MaterialBuilder::set_attributes;
        mtb["build"] = &MaterialBuilder::build;
    }

    static void bind_model(sol::state& env) {
        auto mdl = env.new_usertype<ModelComp>( "Model", 
                sol::constructors<ModelComp(),
                                  ModelComp(Mesh, Material, size_t)>());

        mdl["material"] = &ModelComp::material;
        mdl["mesh"] = &ModelComp::material;
        mdl["layer"] = &ModelComp::material;
    }

    static void bind_hierarchy(sol::state& env) {
        auto prt = env.new_usertype<ParentComp>( "Parent", 
                sol::constructors<ParentComp(),
                                  ParentComp(Entity)>());
        prt["parent"] = &ParentComp::parent;


        auto chd = env.new_usertype<ChildrenComp>( "Children", 
                sol::constructors<ChildrenComp()>());

        chd["children"] = &ChildrenComp::children;
    }



    static void bind_glm(sol::state& env) {

        auto v2 = env.new_usertype<glm::vec2>( "vec2", 
                sol::constructors<glm::vec2(float, float), glm::vec2(float), glm::vec2()>());
        v2["x"] = &glm::vec2::x;
        v2["y"] = &glm::vec2::y;

        v2[sol::meta_function::addition] = [](const glm::vec2* l, const glm::vec2* r) { return *l + *r; };
        v2[sol::meta_function::subtraction] = [](const glm::vec2* l, const glm::vec2* r) { return *l - *r; };
        v2[sol::meta_function::multiplication] = [](const glm::vec2* l, const glm::vec2& r) { return *l * r; };
        v2[sol::meta_function::multiplication] = [](const glm::vec2* l, const float& r) { return *l * r; };

        
        auto v3 = env.new_usertype<glm::vec3>( "vec3", 
                sol::constructors<glm::vec3(glm::vec2, float), glm::vec3(float, float, float), glm::vec3(float), glm::vec3()>());
        v3[sol::meta_function::addition] = [](const glm::vec3* l, const glm::vec3* r) { return *l + *r; };
        v3[sol::meta_function::subtraction] = [](const glm::vec3* l, const glm::vec3* r) { return *l - *r; };
        v3[sol::meta_function::multiplication] = [](const glm::vec3* l, const glm::vec3& r) { return *l * r; };
        v3[sol::meta_function::multiplication] = [](const glm::vec3* l, const float& r) { return *l * r; };

        v3["x"] = &glm::vec3::x;
        v3["y"] = &glm::vec3::y;
        v3["z"] = &glm::vec3::z;

        auto v4 = env.new_usertype<glm::vec4>( "vec4", 
                sol::constructors<glm::vec4(glm::vec3, float),glm::vec4(float,float, float, float), glm::vec4(float), glm::vec4()>());

        v4[sol::meta_function::addition] = [](const glm::vec4* l, const glm::vec4* r) { return *l + *r; };
        v4[sol::meta_function::subtraction] = [](const glm::vec4* l, const glm::vec4* r) { return *l - *r; };
        v4[sol::meta_function::multiplication] = [](const glm::vec4* l, const glm::vec4& r) { return *l * r; };
        v4[sol::meta_function::multiplication] = [](const glm::vec4* l, const float& r) { return *l * r; };

        v4["x"] = &glm::vec4::x;
        v4["y"] = &glm::vec4::y;
        v4["z"] = &glm::vec4::z;
        v4["w"] = &glm::vec4::w;

    }
}
