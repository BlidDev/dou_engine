#include "component.h"
#include "components/primitive.h"
#include "systems.h"
#include <yaml-cpp/yaml.h>
#include "scene.h"
#include "entity.h"
#include "yaml-cpp/emittermanip.h"
#include <yaml-cpp/yaml.h>

namespace engine 
{

    Entity Scene::create_entity() {
        return Entity(this, registry.create());
    }

    SceneManager::SceneManager() {
        current = "NONE";
    }

    void SceneManager::register_scene(const char* name, Scene* scene) {
        if (scenes.contains(name))
            assert("ERROR: Scene name already exists");
        scene->manager = this;
        scenes.insert(std::make_pair(name, scene));
    }

    Scene* SceneManager::get_scene(const char* name) {
        Scene* ptr = nullptr;
        if (!scenes.contains(name))
            assert("ERROR: Scene name does not exist");
        ptr = scenes.at(name);
        return ptr;
    }


    void SceneManager::set_current(const char* name) {
        switched = true;
        if (current != "NONE") {
            Scene* old = get_current();
            end_scene(old);
        }
        if(get_scene(name))
            current = name;
    }


    void SceneManager::end_scene(Scene* scene) {
        scene->on_end();
        end_actions(scene->registry);
        scene->registry.clear();
    }

    Scene* SceneManager::get_current() {
        Scene* ptr = nullptr;
        if (current == "NONE")
            assert("ERROR: Current scene not set");
        ptr = scenes.at(current);
        return ptr;
    }

    void SceneManager::end() {
        if ("NONE" != current)
            get_current()->on_end();
    }

    SceneManager::~SceneManager() {
        for (auto& scene : scenes) {
            delete scene.second;
        }
    }

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
            out<<YAML::Key<<"Position"<<transform.position;
            out<<YAML::Key<<"Size"<<transform.size;
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

        out<<YAML::EndMap;
    }
    void SceneManager::write_scene_to_file(const char* path, Scene* scene) {
        YAML::Emitter out;
        out<<YAML::BeginMap;
        out<<YAML::Key<<"Scene"<<YAML::Value<<scene->name;
        out<<YAML::Key<<"Entities"<<YAML::Value<<YAML::BeginSeq;

        for (auto e : scene->registry.view<entt::entity>()) {
            Entity entity(scene, e);
            write_entity_to_file(out, entity);
            if (!entity)
                return;
            write_entity_to_file(out, entity);
        }

        out<<YAML::EndSeq;
        out<<YAML::EndMap;
    }

    Scene* SceneManager::scene_from_file(const char* path) {
        Scene* scene = new Scene("");

        return scene;
    }
    

}
