#include "components/action.h"
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


    static void write_entity_to_file(YAML::Emitter& out, Entity& entity) {
        out<<YAML::BeginMap;
        out<<YAML::Key<<"Entity"<<YAML::Value<<"1234";

        if (entity.has_component<TagComp>()) {
            out<<YAML::Key<<"TagComp"<<YAML::BeginMap;
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
