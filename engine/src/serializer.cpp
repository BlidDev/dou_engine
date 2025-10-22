#include <espch.h>
#include "component.h"
#include "entity.h"
#include "manager.h"

namespace YAML {
    template<>
    struct convert<engine::UUID> {
        static Node encode(const engine::UUID& uuid) {
            Node node;
            node.push_back((uint32_t)uuid);
            return node;
        }

        static bool decode(const Node& node, engine::UUID& uuid) {
            if(!node.IsScalar())
                return false;
            uuid = node.as<uint64_t>();
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
            out<<YAML::Key<<"Position"<<YAML::Value<<transform.position;
            out<<YAML::Key<<"Size"<<YAML::Value<<transform.size;
            out<<YAML::Key<<"Rotation"<<YAML::Value<<transform.rotation;
            out<<YAML::EndMap;
        }

        if (entity.has_component<ModelComp>()) {
            out<<YAML::Key<<"Model"<<YAML::BeginMap;
                auto& m = entity.get_component<ModelComp>();
                out<<YAML::Key<<"Model Name"<<YAML::Value<<m.model.name;
                out<<YAML::Key<<"Material"<<YAML::BeginMap;
                    out<<YAML::Key<<"Shader"<<YAML::Value<<m.material.shader.path;
                    out<<YAML::Key<<"Filled"<<YAML::Value<<((MODEL_FILLED & m.material.attributes) == MODEL_FILLED);
                    out<<YAML::Key<<"Ambient"<<YAML::Value<<m.material.ambient;
                    out<<YAML::Key<<"Diffuse"<<YAML::Value<<m.material.diffuse;
                    out<<YAML::Key<<"Specular"<<YAML::Value<<m.material.specular;
                    out<<YAML::Key<<"Shininess"<<YAML::Value<<m.material.shininess;
                    out<<YAML::Key<<"Texture"<<YAML::Value<<m.material.texture.path;
                    out<<YAML::Key<<"Textured"<<YAML::Value<<((MODEL_TEXTURED & m.material.attributes) == MODEL_TEXTURED);
                    out<<YAML::Key<<"Immune"<<YAML::Value<<((MODEL_IMMUNE & m.material.attributes) == MODEL_IMMUNE);
                    out<<YAML::Key<<"Layer"<<YAML::Value<<m.layer;
                out<<YAML::EndMap;


            out<<YAML::EndMap;
        }

        if (entity.has_component<ActionsComp>()) {
            out<<YAML::Key<<"Native Actions"<<YAML::BeginSeq;
                ActionsComp& a = entity.get_component<ActionsComp>();
                for (auto action : a.actions) {
                    out<<YAML::BeginMap;
                        out<<YAML::Key<<"Name"<<YAML::Value<<action->inner_name;
                        action->serialize(out);
                    out<<YAML::EndMap;
                }
            out<<YAML::EndSeq;
        }

        //if (entity.has_component<TextComp>()) {
        //    out<<YAML::Key<<"Text"<<YAML::BeginMap;
        //        auto& text = entity.get_component<TextComp>();
        //        out<<YAML::Key<<"Body"<<YAML::Value<<text.body;
        //        out<<YAML::Key<<"Font Size"<<YAML::Value<<text.font_size;
        //        out<<YAML::Key<<"Color"<<YAML::Value<<text.color;
        //    out<<YAML::EndMap;
        //}

        if (entity.has_component<PhysicsBodyComp>()) {
            out<<YAML::Key<<"PhysicsBody"<<YAML::BeginMap;
                auto& ph = entity.get_component<PhysicsBodyComp>();
                out<<YAML::Key<<"Gravity"<<YAML::Value<<ph.gravity;
                out<<YAML::Key<<"Velocity"<<YAML::Value<<ph.velocity;
                out<<YAML::Key<<"Accelaration"<<YAML::Value<<ph.acceleration;
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
                out<<YAML::Key<<"Target"<<YAML::Value<<c.target;
                out<<YAML::Key<<"Up"<<YAML::Value<<c.up;
                out<<YAML::Key<<"FovY"<<YAML::Value<<c.fovy;
                out<<YAML::Key<<"Projection"<<YAML::Value<<(int)c.projection;
                out<<YAML::Key<<"Max Distance"<<YAML::Value<<c.max_distance;
            out<<YAML::EndMap;
        }

        if (entity.has_component<LuaActionComp>()) {
            auto& l = entity.get_component<LuaActionComp>();
            out<<YAML::Key<<"Lua Actions"<<YAML::BeginSeq;
                for (auto scp : l.scripts) {
                    out<<YAML::BeginMap;
                    out<<YAML::Key<<"Path"<<scp.path;
                    out<<YAML::Key<<"Fields"<<YAML::BeginMap;
                        for (auto& [k,v] : scp.env) {
                            if ( !k.is<std::string>() || v.is<sol::function>() || !v.is<UUID>())
                                continue;
                            std::string str = k.as<std::string>();
                            UUID uuid = v.as<UUID>();
                            out<<YAML::Key<<str<<YAML::Value<<uuid.get_uuid();
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
            out<<YAML::Key<<"PntLight"<<YAML::BeginMap;
                out<<YAML::Key<<"Color"<<YAML::Value<<l.color;
                out<<YAML::Key<<"Direction"<<YAML::Value<<l.color;
                out<<YAML::Key<<"Constant"<<YAML::Value<<l.constant;
                out<<YAML::Key<<"Linear"<<YAML::Value<<l.linear;
                out<<YAML::Key<<"Quadratic"<<YAML::Value<<l.quadratic;
                out<<YAML::Key<<"Cut off"<<YAML::Value<<l.cutoff;
                out<<YAML::Key<<"Outer cut off"<<YAML::Value<<l.outer_cutoff;
            out<<YAML::EndMap;
        }

        //if (entity.has_component<ParentComp>()) {
        //    auto& p = entity.get_component<ParentComp>();
        //    out<<YAML::Key<<"Parent"<<YAML::BeginMap;
        //        out<<YAML::Key<<"Parent"<<YAML::Value<<p.parent.uuid();
        //    out<<YAML::EndMap;
        //}

        if (entity.has_component<ParentComp>()) {
            auto& p = entity.get_component<ParentComp>();
            out<<YAML::Key<<"Parent"<<YAML::BeginMap;
                out<<YAML::Key<<"Parent"<<YAML::Value<<p.parent.uuid();
            out<<YAML::EndMap;
        }

        if (entity.has_component<ChildrenComp>()) {
            auto& c = entity.get_component<ChildrenComp>();
            out<<YAML::Key<<"Chidren"<<YAML::BeginSeq;
            for (auto& child : c.children) {
                out<<YAML::Value<<child;
            }
            out<<YAML::EndSeq;
        }

        out<<YAML::EndMap;


    }

    static void read_entity_from_file(YAML::Node& entity, Scene* scene) {
        uint64_t uuid = entity["Entity"].as<uint64_t>(); 

        Entity read_entity = scene->create_entity_with_uuid(uuid);

        auto tag_comp = entity["Tag"];
        if (tag_comp)
            read_entity.add_component<TagComp>(tag_comp["Tag"].as<std::string>());

        auto transform_comp = entity["Transform"];
        if (transform_comp) {
            TransformComp& tc = read_entity.add_component<TransformComp>();
            tc.position = transform_comp["Position"].as<glm::vec3>();
            tc.size = transform_comp["Size"].as<glm::vec3>();
            tc.rotation = transform_comp["Rotation"].as<glm::vec3>();
        }

        auto model_comp = entity["Model"];
        if (model_comp) {
            ModelComp& m = read_entity.add_component<ModelComp>();
            std::string model_name = model_comp["Model Name"].as<std::string>();
            m.model = scene->get_model(model_name.c_str());
            auto material = model_comp["Material"];
            std::string shader_name = material["Shader"].as<std::string>();
            m.material.shader = scene->get_shader(shader_name.c_str());
            m.layer = model_comp["Layer"].as<size_t>();
            EG_ASSERT(m.layer > MAX_RENDER_LAYERS, "Invalid layer number given: {}", m.layer);

            std::string texture_path = material["Texture"].as<std::string>();
            bool filled =   material["Filled"].as<bool>();
            bool textured = material["Textured"].as<bool>();
            bool immune = material["Immune"].as<bool>();
            m.material.attributes |=  filled   ?  MODEL_FILLED : 0;
            m.material.attributes |=  textured ?  MODEL_TEXTURED : 0;
            m.material.attributes |=  immune   ?  MODEL_IMMUNE : 0;


            if (textured)
                m.material.texture = scene->get_texture(texture_path.c_str());
            if (material["Color"]) {
                glm::vec3 color = material["Color"].as<glm::vec3>();
                m.material.ambient = color;
                m.material.diffuse = color;
                m.material.specular = {1.0f, 1.0f, 1.0f};
                m.material.shininess = 32.0f;
            }
            else {
                m.material.ambient = material["Ambient"].as<glm::vec3>();
                m.material.diffuse = material["Diffuse"].as<glm::vec3>();
                m.material.specular = material["Specular"].as<glm::vec3>();
                m.material.shininess = material["Shininess"].as<float>();
            }
            //m.material.print();
        }

        auto actions_comp = entity["Native Actions"];
        if (actions_comp) {
            ActionsComp& ac = read_entity.add_component<ActionsComp>();
            for (auto action : actions_comp) {
                std::string action_name = action["Name"].as<std::string>();
                UpdateComp* a = ac.add(action_name.c_str()).get_last();
                a->dserialize(action);
            }
        }
        //auto text = entity["Text"];
        //if(text) {
        //    TextComp& t = read_entity.add_component<TextComp>();
        //    t.body = text["Body"].as<std::string>();
        //    t.font_size = text["Font Size"].as<int>();
        //    t.color = text["Color"].as<Color>();
        //}

        auto physicbody = entity["PhysicsBody"];
        if(physicbody) {
            PhysicsBodyComp& ph = read_entity.add_component<PhysicsBodyComp>(); 
            ph.gravity = physicbody["Gravity"].as<float>();
            ph.velocity = physicbody["Velocity"].as<glm::vec3>();
            ph.is_solid = physicbody["Is Solid"].as<bool>();
            ph.is_static = physicbody["Is Static"].as<bool>();
            ph.move_delta = physicbody["Move Delta"].as<glm::vec3>();
            auto inter = physicbody["Intersect Callback"];
            if (inter) {
                ph.lua_callback = inter.as<LuaCallback>();
            }
        }

        auto camera = entity["Camera"];
        if(camera) {
            CameraComp& c = read_entity.add_component<CameraComp>();
            c.target = camera["Target"].as<glm::vec3>();
            c.up = camera["Up"].as<glm::vec3>();
            c.fovy = camera["FovY"].as<float>();
            c.projection = (CameraProjection)camera["Projection"].as<int>();
            c.max_distance = camera["Max Distance"].as<float>();
            if (read_entity.has_component<TransformComp>()) {
                c.last_pos = read_entity.get_component<TransformComp>().position;
            }
        }

        auto lua_actions = entity["Lua Actions"];
        if (lua_actions) {
            auto& ls = read_entity.add_component<LuaActionComp>(LuaActionComp(UUID(uuid)));
            for (auto m : lua_actions) {
                std::string path = m["Path"].as<std::string>();
                ls.add(scene,path);

                for (auto f : m["Fields"]) {
                    std::string name = f.first.as<std::string>();
                    uint64_t id = f.second.as<uint64_t>();
                    ls.bind_field(name, UUID(id));
                }
            }
        }

        auto dlight = entity["DirLight"];
        if(dlight) {
            auto& l = read_entity.add_component<DirLightComp>();
            l.direction = dlight["Direction"].as<glm::vec3>();
            l.ambient = dlight["Ambient"].as<float>();
            l.color = dlight["Color"].as<glm::vec3>();
            l.strength = dlight["Strength"].as<float>();
        }

        auto plight = entity["PntLight"];
        if(plight) {
            auto& l = read_entity.add_component<PntLightComp>();
            l.color = plight["Color"].as<glm::vec3>();
            l.constant = plight["Constant"].as<float>();
            l.linear = plight["Linear"].as<float>();
            l.quadratic = plight["Quadratic"].as<float>();
        }

        auto slight = entity["SptLight"];
        if(slight) {
            auto& l = read_entity.add_component<SptLightComp>();
            l.color = slight["Color"].as<glm::vec3>();
            l.direction = slight["Direction"].as<glm::vec3>();
            l.constant = slight["Constant"].as<float>();
            l.linear = slight["Linear"].as<float>();
            l.quadratic = slight["Quadratic"].as<float>();

            l.cutoff =       glm::cos(glm::radians(slight["Cut off"].as<float>()));
            l.outer_cutoff = glm::cos(glm::radians(slight["Outer cut off"].as<float>()));
        }


        //auto parent = entity["Parent"];
        //if (parent) {
        //    UUID tmp = parent["Parent"].as<UUID>();
        //    read_entity.make_child_of(tmp);
        //}

        auto children = entity["Children"];
        if (children) {
            for (auto child : children) {
                UUID c = child.as<UUID>();
                read_entity.add_child(c);
            }
        }

    }

    void SceneManager::write_scene_to_file(const char* path, Scene* scene) {
        std::ofstream file(path);
        YAML::Emitter out;
        out<<YAML::BeginMap;
        out<<YAML::Key<<"Scene"<<YAML::Value<<scene->name;

        out<<YAML::Key<<"Render Data"<<YAML::BeginMap;
            out<<YAML::Key<<"Main Camera"<<YAML::Value<<(uint64_t)scene->main_camera;
            out<<YAML::Key<<"Ambient"<<YAML::Value<<scene->ambient;
            out<<YAML::Key<<"Ambient Strength"<<YAML::Value<<scene->ambient_strength;
        out<<YAML::EndMap;

        out<<YAML::Key<<"Entities"<<YAML::Value<<YAML::BeginSeq;

        for (auto e : scene->registry.view<entt::entity>()) {
            Entity entity(scene, e);
            if (!entity)
                return;
            write_entity_to_file(out, entity);
        }

        out<<YAML::EndSeq;
        out<<YAML::EndMap;
        file<<out.c_str();
        file.close();
    }

    // FIXME
    Scene* SceneManager::scene_from_file(const char* path) {
        std::ifstream stream(path);
        std::stringstream str_stream;
        str_stream<<stream.rdbuf();

        YAML::Node data = YAML::Load(str_stream.str());

        EG_ASSERT(!data["Scene"],"Cannot read {}, scene does not exist", path );

        std::string scene_name = data["Scene"].as<std::string>();
        Scene* scene = new Scene(scene_name);

       
        EG_ASSERT(!data["Render Data"], "Section Render Data for scene {} not found", scene_name);

        auto render_data = data["Render Data"];
        scene->main_camera = UUID(render_data["Main Camera"].as<uint64_t>());
        scene->ambient = render_data["Ambient"].as<glm::vec3>();
        scene->ambient_strength = render_data["Ambient Strength"].as<float>();


        EG_ASSERT(!data["Entities"], "Section Entities for scene {} not found", scene_name);
        auto entities = data["Entities"];

        for (auto entity : entities) {
            read_entity_from_file(entity, scene);
        }


        EG_CORE_INFO("Finished reading scene {}", scene_name.c_str());

        return scene;
    }

    void Scene::add_from_file(const char* path) {
        std::ifstream stream(path);
        std::stringstream str_stream;
        str_stream<<stream.rdbuf();

        YAML::Node data = YAML::Load(str_stream.str());

        EG_ASSERT(!data["Scene"],"Cannot read {}, scene does not exist", path );

        std::string scene_name = data["Scene"].as<std::string>();
        name = scene_name;

        EG_ASSERT(!data["Render Data"], "Section Render Data for scene {} not found", scene_name);

        auto render_data = data["Render Data"];
        main_camera = UUID(render_data["Main Camera"].as<uint64_t>());
        ambient = render_data["Ambient"].as<glm::vec3>();
        ambient_strength = render_data["Ambient Strength"].as<float>();

        EG_ASSERT(!data["Entities"], "Section Entities for scene {} not found", scene_name);
        auto entities = data["Entities"];

        for (auto entity : entities) {
            read_entity_from_file(entity, this);
        }

        EG_CORE_INFO("Finished adding to scene {}", scene_name.c_str());
    }

}
