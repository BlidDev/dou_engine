#include "scene.h"
#include <yaml-cpp/yaml.h>
#include "component.h"
#include "entity.h"
#include <raylib.h>
#include <fstream>


namespace YAML {
    template <>
    struct convert<Vector3> {
        static Node encode(const Vector3& rhs) {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            return node;
        }

        static bool decode(const Node& node, Vector3& rhs) {
            if(!node.IsSequence() || node.size() != 3)
                return false;
            rhs.x = node[0].as<float>();
            rhs.x = node[1].as<float>();
            rhs.x = node[2].as<float>();
            return true;
        }
    };
}

namespace engine {

    YAML::Emitter& operator<<(YAML::Emitter& out, const Vector3& v) {
        out<<YAML::Flow;
        out<<YAML::BeginSeq<<v.x<<v.y<<v.z<<YAML::EndSeq;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const Color& c) {
        out<<YAML::Flow;
        out<<YAML::BeginSeq<<c.r<<c.g<<c.b<<c.a<<YAML::EndSeq;
        return out;
    }


    static void write_entity_to_file(YAML::Emitter& out, Entity& entity) {
        out<<YAML::BeginMap;
        out<<YAML::Key<<"Entity"<<YAML::Value<<"1234";

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
            out<<YAML::EndMap;
        }

        if (entity.has_component<PrimitiveComp>()) {
            out<<YAML::Key<<"Primitive"<<YAML::BeginMap;
                auto& p = entity.get_component<PrimitiveComp>();
                out<<YAML::Key<<"Color"<<YAML::Value<<p.color;
                const char* shapes[] = {"PLANE","CUBE","SPHERE"};
                out<<YAML::Key<<"Shape"<<YAML::Value<<shapes[(int)p.shape];
                out<<YAML::Key<<"Attributes"<<YAML::BeginMap;
                    out<<YAML::Key<<"Filled"<<YAML::Value<<((PRIMITVE_FILLED & p.attributes) == PRIMITVE_FILLED);
                    out<<YAML::Key<<"Wireframe"<<YAML::Value<<((PRIMITVE_WIREFRAME & p.attributes) == PRIMITVE_WIREFRAME);
                    out<<YAML::Key<<"Immune"<<YAML::Value<<((PRIMITVE_IMMUNE & p.attributes) == PRIMITVE_IMMUNE);
                    out<<YAML::EndMap;
            out<<YAML::EndMap;
        }

        if (entity.has_component<ActionsComp>()) {
            out<<YAML::Key<<"Actions"<<YAML::BeginMap;
                out<<YAML::Key<<"Actions"<<YAML::BeginSeq<<"Todo"<<YAML::EndSeq;
            out<<YAML::EndMap;
        }

        if (entity.has_component<TextComp>()) {
            out<<YAML::Key<<"Text"<<YAML::BeginMap;
                auto& text = entity.get_component<TextComp>();
                out<<YAML::Key<<"Body"<<YAML::Value<<text.body;
                out<<YAML::Key<<"Font Size"<<YAML::Value<<text.font_size;
                out<<YAML::Key<<"Color"<<YAML::Value<<text.color;
            out<<YAML::EndMap;
        }

        if (entity.has_component<PhysicsBodyComp>()) {
            out<<YAML::Key<<"PhysicsBody"<<YAML::BeginMap;
                auto& ph = entity.get_component<PhysicsBodyComp>();
                out<<YAML::Key<<"Gravity"<<YAML::Value<<ph.gravity;
                out<<YAML::Key<<"Velocity"<<YAML::Value<<ph.velocity;
                out<<YAML::Key<<"Accelaration"<<YAML::Value<<ph.acceleration;
                out<<YAML::Key<<"Is Solid"<<YAML::Value<<ph.is_solid;
                out<<YAML::Key<<"Is Static"<<YAML::Value<<ph.is_static;
                out<<YAML::Key<<"Move Delta"<<YAML::Value<<ph.move_delta;
            out<<YAML::EndMap;
        }

        if (entity.has_component<Camera>()) {
            out<<YAML::Key<<"Camera"<<YAML::BeginMap;
                auto& c = entity.get_component<Camera>();
                out<<YAML::Key<<"Position"<<YAML::Value<<c.position;
                out<<YAML::Key<<"Target"<<YAML::Value<<c.target;
                out<<YAML::Key<<"Up"<<YAML::Value<<c.up;
                out<<YAML::Key<<"FovY"<<YAML::Value<<c.fovy;
                out<<YAML::Key<<"Projection"<<YAML::Value<<(int)c.projection;
            out<<YAML::EndMap;
        }

        out<<YAML::EndMap;
    }
    void SceneManager::write_scene_to_file(const char* path, Scene* scene) {
        std::ofstream file(path);
        YAML::Emitter out;
        out<<YAML::BeginMap;
        out<<YAML::Key<<"Scene"<<YAML::Value<<scene->name;
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

    Scene* SceneManager::scene_from_file(const char* path) {
        std::ifstream stream(path);
        std::stringstream str_stream;
        str_stream<<stream.rdbuf();

        YAML::Node data = YAML::Load(str_stream.str());

        if (!data["Scene"])
            return nullptr;

        std::string scene_name = data["Scene"].as<std::string>();
        Scene* scene = new Scene(scene_name);

        auto entities = data["Entities"];

        for (auto entity : entities) {
            uint64_t uuid = entity["Entity"].as<uint64_t>();

            Entity read_entity = scene->create_entity();

            auto tag_comp = entity["Tag"];
            if (tag_comp)
                read_entity.add_component<TagComp>(tag_comp["Tag"].as<std::string>());

            auto transform_comp = entity["Transform"];
            if (transform_comp) {
                TransformComp& tc = read_entity.add_component<TransformComp>();
                tc.position = transform_comp["Position"].as<Vector3>();
                tc.size = transform_comp["Size"].as<Vector3>();
            }

        }


        return scene;
    }

}
