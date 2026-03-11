#include <espch.h>
#include "component.h"
#include "entity.h"
#include "manager.h"

namespace YAML {
    template<>
    struct convert<engine::UUID> {
        static Node encode(const engine::UUID& uuid) {
            return Node((uint64_t)uuid);
        }

        static bool decode(const Node& node, engine::UUID& uuid) {
            if(!node.IsScalar())
                return false;
            uuid = node.as<uint64_t>();
            return true;
        }

    };
    template <>
    struct convert<glm::vec2> {
        static Node encode(const glm::vec2& rhs) {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            return node;
        }

        static bool decode(const Node& node, glm::vec2& rhs) {
            if(!node.IsSequence() || node.size() != 2)
                return false;
            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            return true;
        }
    };

    template <>
    struct convert<glm::vec3> {
        static Node encode(const glm::vec3& rhs) {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            return node;
        }

        static bool decode(const Node& node, glm::vec3& rhs) {
            if(!node.IsSequence() || node.size() != 3)
                return false;
            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            return true;
        }
    };

    template <>
    struct convert<engine::LuaCallback> {
        static Node encode(const engine::LuaCallback& rhs) {
            Node node;
            node.push_back(rhs.path);
            node.push_back(rhs.function);
            return node;
        }

        static bool decode(const Node& node, engine::LuaCallback& rhs) {
            if(!node.IsSequence() || node.size() != 2)
                return false;
            rhs.path     = node[0].as<std::string>();
            rhs.function = node[1].as<std::string>();
            return true;
        }
    };

    template <>
    struct convert<glm::vec4> {
        static Node encode(const glm::vec4& rhs) {
            Node node;
            node.push_back((float)rhs.r);
            node.push_back((float)rhs.g);
            node.push_back((float)rhs.b);
            node.push_back((float)rhs.a);
            return node;
        }

        static bool decode(const Node& node, glm::vec4& rhs) {
            if(!node.IsSequence() || node.size() != 4)
                return false;
            rhs.r = (float)node[0].as<float>();
            rhs.g = (float)node[1].as<float>();
            rhs.b = (float)node[2].as<float>();
            rhs.a = (float)node[3].as<float>();
            return true;
        }
    };
}

namespace engine {

    template <typename T>
    static void make_field_node(YAML::Emitter& out,T field, std::string ident) {
        out<<YAML::Flow;
        out<<YAML::BeginSeq<<ident<<YAML::Node(field)<<YAML::EndSeq;
    }

    #define EMMIT_FIELD(name,var, type, ident) if (var.is<type>())  {out<<YAML::Key<<name; make_field_node(out, var.as<type>(), ident);}

    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v) {
        out<<YAML::Flow;
        out<<YAML::BeginSeq<<v.x<<v.y<<YAML::EndSeq;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v) {
        out<<YAML::Flow;
        out<<YAML::BeginSeq<<v.x<<v.y<<v.z<<YAML::EndSeq;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& c) {
        out<<YAML::Flow;
        out<<YAML::BeginSeq<<c.r<<c.g<<c.b<<c.a<<YAML::EndSeq;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const LuaCallback& l) {
        out<<YAML::Flow;
        out<<YAML::BeginSeq<<l.path<<l.function<<YAML::EndSeq;
        return out;
    }

    static void write_entity_to_file(YAML::Emitter& out, Entity& entity) {
        out<<YAML::BeginMap;
        out<<YAML::Key<<"Entity"<<YAML::Value<<entity.uuid();

        if (entity.has_component<TagComp>()) {
            out<<YAML::Key<<"Tag"<<YAML::BeginMap;
            auto& tag = entity.get_component<TagComp>();
            out<<YAML::Key<<"Tag"<<YAML::Value<<tag.tag;
            out<<YAML::EndMap;
        }

        if (entity.has_component<TransformComp>()) {
            out<<YAML::Key<<"Transform"<<YAML::BeginMap;
            auto& transform = entity.get_component<TransformComp>();
            out<<YAML::Key<<"Position"<<YAML::Value<<transform.position();
            out<<YAML::Key<<"Size"<<YAML::Value<<transform.size();
            out<<YAML::Key<<"Rotation"<<YAML::Value<<transform.rotation();
            out<<YAML::EndMap;
        }

        if (entity.has_component<ModelComp>()) {
            out<<YAML::Key<<"Model"<<YAML::BeginMap;
                auto& m = entity.get_component<ModelComp>();
                out<<YAML::Key<<"Mesh Name"<<YAML::Value<<m.mesh.name;
                out<<YAML::Key<<"Layer"<<YAML::Value<<m.layer;
                out<<YAML::Key<<"Immune"<<YAML::Value<<m.is_immune;
                out<<YAML::Key<<"Material"<<YAML::BeginMap;
                    out<<YAML::Key<<"Shader"<<YAML::Value<<m.material.shader.path;
                    out<<YAML::Key<<"Ambient"<<YAML::Value<<m.material.ambient;
                    out<<YAML::Key<<"Diffuse"<<YAML::Value<<m.material.diffuse;
                    out<<YAML::Key<<"Specular"<<YAML::Value<<m.material.specular;
                    out<<YAML::Key<<"Shininess"<<YAML::Value<<m.material.shininess;
                    out<<YAML::Key<<"Texture"<<YAML::Value<<m.material.texture.path;
                    out<<YAML::Key<<"Is Textured"<<YAML::Value<<m.material.is_textured;
                    out<<YAML::Key<<"Texture Repeats"<<YAML::Value<<m.material.tex_repeat;
                out<<YAML::EndMap;


            out<<YAML::EndMap;
        }

        if (entity.has_component<ActionsComp>()) {
            out<<YAML::Key<<"Native Actions"<<YAML::BeginSeq;
                ActionsComp& a = entity.get_component<ActionsComp>();
                for (auto& action : a.actions) {
                    out<<YAML::BeginMap;
                        out<<YAML::Key<<"Name"<<YAML::Value<<action->inner_name;
                        action->serialize(out);
                    out<<YAML::EndMap;
                }
            out<<YAML::EndSeq;
        }

        if (entity.has_component<PhysicsBodyComp>()) {
            out<<YAML::Key<<"PhysicsBody"<<YAML::BeginMap;
                auto& ph = entity.get_component<PhysicsBodyComp>();
                out<<YAML::Key<<"Gravity"<<YAML::Value<<ph.gravity;
                out<<YAML::Key<<"Mass"<<YAML::Value<<ph.mass;
                out<<YAML::Key<<"Slipperiness"<<YAML::Value<<ph.slipperiness;
                out<<YAML::Key<<"Velocity"<<YAML::Value<<ph.velocity;
                out<<YAML::Key<<"Is Solid"<<YAML::Value<<ph.is_solid;
                out<<YAML::Key<<"Is Static"<<YAML::Value<<ph.is_static;
                out<<YAML::Key<<"Move Delta"<<YAML::Value<<ph.move_delta;
                if (ph.lua_callback) {
                    out<<YAML::Key<<"Intersect Callback"<<YAML::Value<<ph.lua_callback;
                }
            out<<YAML::EndMap;
        }

        if (entity.has_component<CameraComp>()) {
            out<<YAML::Key<<"Camera"<<YAML::BeginMap;
                auto& c = entity.get_component<CameraComp>();
                out<<YAML::Key<<"Pitch"<<YAML::Value<<c.pitch();
                out<<YAML::Key<<"Yaw"<<YAML::Value<<c.yaw();
                out<<YAML::Key<<"Up"<<YAML::Value<<c.up;
                out<<YAML::Key<<"FovY"<<YAML::Value<<c.fovy;
                const char* names[] = {"Perspective", "Orthographic"};
                out<<YAML::Key<<"Projection"<<YAML::Value<<names[(int)c.projection_mode];
                out<<YAML::Key<<"Max Distance"<<YAML::Value<<c.max_distance;
                glm::vec2 size = entity.scene_ptr()->manager->main_window.size();
                if (size != c.framebuffer.last_scale) {
                    out<<YAML::Key<<"Framebuffer"<<YAML::BeginMap;
                        out<<YAML::Key<<"Width"<<YAML::Value<<c.framebuffer.last_scale.x;
                        out<<YAML::Key<<"Height"<<YAML::Value<<c.framebuffer.last_scale.y;
                    out<<YAML::EndMap;
                }
                out<<YAML::Key<<"Present Shader"<<YAML::Value<<c.present_shader.path;

            out<<YAML::EndMap;
        }

        if (entity.has_component<LuaActionComp>()) {
            auto& l = entity.get_component<LuaActionComp>();
            out<<YAML::Key<<"Lua Actions"<<YAML::BeginSeq;
                for (const auto& scp : l.scripts) {
                    out<<YAML::BeginMap;
                    out<<YAML::Key<<"Path"<<scp.path;
                    out<<YAML::Key<<"Fields"<<YAML::BeginMap;
                        for (const auto& [k,v] : scp.env) {
                            if ( !k.is<std::string>() || v.is<sol::function>())
                                continue;
                            
                            std::string str = k.as<std::string>();
                            EMMIT_FIELD(str, v, UUID, "u")
                            else EMMIT_FIELD(str, v, int,   "i")
                            else EMMIT_FIELD(str, v, float, "f")
                            else EMMIT_FIELD(str, v, bool, "b")
                            else EMMIT_FIELD(str, v, glm::vec2, "v2")
                            else EMMIT_FIELD(str, v, glm::vec3, "v3")
                            else EMMIT_FIELD(str, v, glm::vec4, "v4")
                            // fucking hell
                        }
                    out<<YAML::EndMap;
                    out<<YAML::EndMap;
                }
            out<<YAML::EndSeq;
        }

        if (entity.has_component<DirLightComp>()) {
            auto& l = entity.get_component<DirLightComp>();
            out<<YAML::Key<<"DirLight"<<YAML::BeginMap;
                out<<YAML::Key<<"Direction"<<YAML::Value<<l.direction;
                out<<YAML::Key<<"Ambient"<<YAML::Value<<l.ambient;
                out<<YAML::Key<<"Color"<<YAML::Value<<l.color;
                out<<YAML::Key<<"Strength"<<YAML::Value<<l.strength;
            out<<YAML::EndMap;
        }

        if (entity.has_component<PntLightComp>()) {
            auto& l = entity.get_component<PntLightComp>();
            out<<YAML::Key<<"PntLight"<<YAML::BeginMap;
                out<<YAML::Key<<"Color"<<YAML::Value<<l.color;
                out<<YAML::Key<<"Constant"<<YAML::Value<<l.constant;
                out<<YAML::Key<<"Linear"<<YAML::Value<<l.linear;
                out<<YAML::Key<<"Quadratic"<<YAML::Value<<l.quadratic;
            out<<YAML::EndMap;
        }

        if (entity.has_component<SptLightComp>()) {
            auto& l = entity.get_component<SptLightComp>();
            out<<YAML::Key<<"SptLight"<<YAML::BeginMap;
                out<<YAML::Key<<"Color"<<YAML::Value<<l.color;
                out<<YAML::Key<<"Direction"<<YAML::Value<<l.direction;
                out<<YAML::Key<<"Constant"<<YAML::Value<<l.constant;
                out<<YAML::Key<<"Linear"<<YAML::Value<<l.linear;
                out<<YAML::Key<<"Quadratic"<<YAML::Value<<l.quadratic;
                out<<YAML::Key<<"Cut off"<<YAML::Value<<l.cutoff;
                out<<YAML::Key<<"Outer cut off"<<YAML::Value<<l.outer_cutoff;
            out<<YAML::EndMap;
        }

        if (entity.has_component<ParentComp>()) {
            auto& p = entity.get_component<ParentComp>();
            out<<YAML::Key<<"Parent"<<YAML::BeginMap;
                out<<YAML::Key<<"Parent"<<YAML::Value<<p.parent.uuid();
            out<<YAML::EndMap;
        }

        if (entity.has_component<ChildrenComp>()) {
            auto& c = entity.get_component<ChildrenComp>();
            out<<YAML::Key<<"Children"<<YAML::BeginSeq;
            for (const auto& child : c.children) {
                out<<YAML::Value<<child;
            }
            out<<YAML::EndSeq;
        }

        out<<YAML::EndMap;


    }

    template <typename T>
    static T safe_read(const YAML::Node& node,const char* name) {
        const auto& tmp = node[name];
        DU_ASSERT(!tmp, "Trying to read node {} for type {} construction but it doesn't exist", name, typeid(T).name());

        try { return tmp.as<T>(); }
        catch (const YAML::Exception& e){
            DU_THROW_ASSERT("Trying to read node {} but the input is invalid: {}", name,e.what());
        }   
    }
    static void read_entity_from_file(YAML::Node& entity, Scene* scene, std::filesystem::path root) {
        DU_ASSERT(!entity["Entity"], "No uuid given to entity");
        uint64_t uuid = safe_read<uint64_t>(entity, "Entity");
        Entity read_entity = scene->create_entity_with_uuid(uuid);

        auto tag_comp = entity["Tag"];
        if (tag_comp)
            read_entity.add_component<TagComp>(safe_read<std::string>(tag_comp, "Tag"));

        auto transform_comp = entity["Transform"];
        if (transform_comp) {
            TransformComp& tc = read_entity.add_component<TransformComp>();
            tc.set_position(safe_read<glm::vec3>(transform_comp,"Position"));
            tc.set_size(safe_read<glm::vec3>(transform_comp,"Size"));
            tc.set_rotation(safe_read<glm::vec3>(transform_comp,"Rotation"));
        }

        auto model_comp = entity["Model"];
        if (model_comp) {
            ModelComp& m = read_entity.add_component<ModelComp>();
            std::string model_name = safe_read<std::string>(model_comp,"Mesh Name");
            m.mesh = scene->get_mesh(model_name.c_str());
            auto material = model_comp["Material"];
            std::string shader_name = safe_read<std::string>(material,"Shader");
            m.material.shader = scene->get_shader(shader_name.c_str());
            m.layer = safe_read<size_t>(model_comp,"Layer");
            m.is_immune = safe_read<bool>(model_comp,"Immune");
            DU_ASSERT(m.layer > MAX_RENDER_LAYERS, "Invalid layer number given: {}", m.layer);

            std::string texture_path = safe_read<std::string>(material,"Texture");
            m.material.is_textured = safe_read<bool>(material,"Is Textured");

            auto tex_repeat = material["Texture Repeats"];

            m.material.tex_repeat = (tex_repeat) ? 
                                tex_repeat.as<glm::vec2>() : 
                                glm::vec2(1.0f);

            if (m.material.is_textured)
                m.material.texture = scene->get_texture(texture_path.c_str());
            if (material["Color"]) {
                glm::vec3 color = safe_read<glm::vec3>(material,"Color");
                m.material.ambient = color;
                m.material.diffuse = color;
                m.material.specular = {1.0f, 1.0f, 1.0f};
                m.material.shininess = 32.0f;
            }
            else {
                m.material.ambient   = safe_read<glm::vec3>(material,"Ambient");
                m.material.diffuse   = safe_read<glm::vec3>(material,"Diffuse");
                m.material.specular  = safe_read<glm::vec3>(material,"Specular");
                m.material.shininess = safe_read<float>(material,"Shininess");
            }
            //m.material.print();
        }

        auto actions_comp = entity["Native Actions"];
        if (actions_comp) {
            ActionsComp& ac = read_entity.add_component<ActionsComp>();
            for (const auto& action : actions_comp) {
                std::string action_name = safe_read<std::string>(action,"Name");
                UpdateComp* a = ac.add(action_name.c_str()).get_last();
                a->dserialize(action);
            }
        }

        auto physicbody = entity["PhysicsBody"];
        if(physicbody) {
            PhysicsBodyComp& ph = read_entity.add_component<PhysicsBodyComp>(); 
            ph.gravity = safe_read<float>(physicbody,"Gravity");
            ph.mass = safe_read<float>(physicbody,"Mass");
            ph.slipperiness = safe_read<float>(physicbody,"Slipperiness");
            ph.velocity = safe_read<glm::vec3>(physicbody,"Velocity");
            ph.is_solid = safe_read<bool>(physicbody,"Is Solid");
            ph.is_static = safe_read<bool>(physicbody,"Is Static");
            ph.move_delta = safe_read<glm::vec3>(physicbody,"Move Delta");
            auto inter = physicbody["Intersect Callback"];
            if (inter) {
                ph.lua_callback = inter.as<LuaCallback>();
            }
        }

        auto camera = entity["Camera"];
        if(camera) {
            CameraComp& c = read_entity.add_component<CameraComp>();
            c.up = safe_read<glm::vec3>(camera,"Up");
            c.set_pitch(safe_read<float>(camera,"Pitch"));
            c.set_yaw  (safe_read<float>(camera,"Yaw"));
            c.fovy = safe_read<float>(camera,"FovY");

            std::string identifier = safe_read<std::string>(camera,"Projection");
            const char* names[] = {"Perspective", "Orthographic", "0", "1"};
            int tmp = -1;
            for (int i = 0; i < 4; i++) {
                if (identifier == names[i])
                    tmp = i % 2;
            }
            DU_ASSERT(tmp == -1, "Unkown CameraProjection identifier {}", identifier);
            c.projection_mode = (CameraProjection)tmp;

            c.max_distance = safe_read<float>(camera,"Max Distance");
            glm::vec2 fb_size = scene->manager->main_window.size();
            auto framebuffer = camera["Framebuffer"];
            if (framebuffer) {
                fb_size.x = safe_read<int>(framebuffer,"Width");
                fb_size.y = safe_read<int>(framebuffer,"Height");
            }

            make_framebuffer(c.framebuffer, fb_size.x, fb_size.y);

            std::string present_shader =  safe_read<std::string>(camera,"Present Shader");
            c.present_shader = scene->get_shader(present_shader.c_str());
        }

        auto lua_actions = entity["Lua Actions"];
        if (lua_actions) {
            auto& ls = read_entity.add_component<LuaActionComp>(LuaActionComp(UUID(uuid)));
            for (const auto& m : lua_actions) {
                std::string path = safe_read<std::string>(m,"Path");
                ls.add(scene,(root / std::filesystem::path(path)));

                for (const auto& f : m["Fields"]) {
                    std::string name = f.first.as<std::string>();
                    DU_ASSERT(!f.second.IsSequence(), " {} - Invaild field decleration", name);
                    std::string type_flag = f.second[0].as<std::string>();
                    YAML::Node var = f.second[1];

                    using FnPtr = void(*)(LuaActionComp&, std::string& ,YAML::Node&);
                    static std::unordered_map<std::string, FnPtr> atlas;

#define ATLAS_ENTRY(entry, type) atlas[entry] = [](LuaActionComp& l, std::string& name, YAML::Node& node) {l.bind_field(name, node.as<type>());};
                    ATLAS_ENTRY("u", UUID);
                    ATLAS_ENTRY("i", int);
                    ATLAS_ENTRY("f", float);
                    ATLAS_ENTRY("b", bool);
                    ATLAS_ENTRY("v2", glm::vec2);
                    ATLAS_ENTRY("v3", glm::vec3);
                    ATLAS_ENTRY("v4", glm::vec4);
#undef ATLAS_ENTRY
                    const auto& operation = atlas.find(type_flag);

                    DU_ASSERT(operation == atlas.end(), "Invaild type flag \'{}\' in lua field {}",type_flag, name); 
                    operation->second(ls, name, var);
                }
            }
        }

        auto dlight = entity["DirLight"];
        if(dlight) {
            auto& l = read_entity.add_component<DirLightComp>();
            l.direction = safe_read<glm::vec3>(dlight,"Direction");
            l.ambient = safe_read<float>(dlight,"Ambient");
            l.color = safe_read<glm::vec3>(dlight,"Color");
            l.strength = safe_read<float>(dlight,"Strength");
        }

        auto plight = entity["PntLight"];
        if(plight) {
            auto& l = read_entity.add_component<PntLightComp>();
            l.color = safe_read<glm::vec3>(plight,"Color");
            l.constant = safe_read<float>(plight,"Constant");
            l.linear = safe_read<float>(plight,"Linear");
            l.quadratic = safe_read<float>(plight,"Quadratic");
        }

        auto slight = entity["SptLight"];
        if(slight) {
            auto& l = read_entity.add_component<SptLightComp>();
            l.color = safe_read<glm::vec3>(slight,"Color");
            l.direction = safe_read<glm::vec3>(slight,"Direction");
            l.constant = safe_read<float>(slight,"Constant");
            l.linear = safe_read<float>(slight,"Linear");
            l.quadratic = safe_read<float>(slight,"Quadratic");

            l.cutoff =       safe_read<float>(slight,"Cut off");
            l.outer_cutoff = safe_read<float>(slight,"Outer cut off");
        }


        //auto parent = entity["Parent"];
        //if (parent) {
        //    UUID tmp = parent["Parent"].as<UUID>();
        //    if (!scene->uuid_to_entity(tmp).is_parent())
        //        read_entity.make_child_of(tmp);
        //}


    }

    void SceneManager::write_scene_to_file(const char* path, Scene* scene) {
        std::ofstream file(path);
        DU_ASSERT(!file.is_open(), "Could not open {}", path);
        YAML::Emitter out;
        out<<YAML::BeginMap;
        out<<YAML::Key<<"Scene"<<YAML::Value<<scene->name;

        out<<YAML::Key<<"Render Data"<<YAML::BeginMap;
            out<<YAML::Key<<"Main Camera"<<YAML::Value<<(uint64_t)scene->main_camera;
            out<<YAML::Key<<"Ambient"<<YAML::Value<<scene->s_render_data.ambient;
            out<<YAML::Key<<"Ambient Strength"<<YAML::Value<<scene->s_render_data.ambient_strength;
            out<<YAML::Key<<"Clear Color"<<YAML::Value<<scene->s_render_data.clear_color;
        out<<YAML::EndMap;

        out<<YAML::Key<<"Entities"<<YAML::Value<<YAML::BeginSeq;
        auto view = scene->registry.view<entt::entity>();
        for (const auto& e : view) {
            Entity entity(scene, e);
            if (!entity)
                continue;
            
            write_entity_to_file(out, entity);
        }

        out<<YAML::EndSeq;
        out<<YAML::EndMap;
        file<<out.c_str();
        file.close();
        DU_CORE_DEBUG_TRACE("Written {}", path);
    }

    static void apply_hierarchy(YAML::Node& entites, Scene* scene) {

        for (const auto& entity : entites) {
            uint64_t uuid = safe_read<uint64_t>(entity,"Entity");
            Entity read_entity = scene->uuid_to_entity(uuid);

            auto children = entity["Children"];
            if (children) {
                for (const auto& child : children) {
                    UUID c = child.as<UUID>();
                    if (get_entities_relation(*scene, uuid, c) == 0)
                        read_entity.add_child(c);
                }
            }

        }
    }

    void Scene::add_from_file(const char* path, std::filesystem::path root) {
        std::ifstream stream(path);
        std::stringstream str_stream;
        str_stream<<stream.rdbuf();

        YAML::Node data = YAML::Load(str_stream.str());

        DU_ASSERT(!data["Scene"],"Cannot read {}, scene does not exist", path );

        std::string scene_name = safe_read<std::string>(data,"Scene");
        name = scene_name;

        DU_ASSERT(!data["Render Data"], "Section Render Data for scene {} not found", scene_name);

        auto render_data = data["Render Data"];
        main_camera = UUID(safe_read<uint64_t>(render_data,"Main Camera"));
        s_render_data.clear_color = safe_read<glm::vec4>(render_data,"Clear Color");
        s_render_data.ambient = safe_read<glm::vec3>(render_data,"Ambient");
        s_render_data.ambient_strength = safe_read<float>(render_data,"Ambient Strength");

        DU_ASSERT(!data["Entities"], "Section Entities for scene {} not found", scene_name);
        auto entities = data["Entities"];

        for (auto entity : entities) {
            try {
                read_entity_from_file(entity, this, root);
            } catch (YAML::Exception e) {
                DU_ASSERT(true, "{}", e.what(), e.mark.line);
            }
            
        }

        apply_hierarchy(entities, this);

        DU_DEBUG_TRACE("Finished adding to scene {}", scene_name.c_str());
    }


    std::string extract_scene_name(const char* path) {
        std::ifstream stream(path);
        DU_ASSERT(stream.fail(), "Could not extract scene name from {}", path);
        std::stringstream str_stream;
        str_stream<<stream.rdbuf();

        YAML::Node data = YAML::Load(str_stream.str());

        DU_ASSERT(!data["Scene"],"Cannot extract from {}, scene name does not exist", path );

        return safe_read<std::string>(data,"Scene");
    }

}
