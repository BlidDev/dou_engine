#include "component.h"
#include "systems.h"
#include "scene.h"
#include "entity.h"

namespace engine 
{

    Entity Scene::create_entity() {
        return create_entity_with_uuid(UUID());
    }

    Entity Scene::create_entity_with_uuid(uint64_t uuid) {
        Entity tmp(this, registry.create());
        tmp.add_component<UUID>(uuid);
        uuids.insert(std::make_pair(uuid, tmp.id()));
        return tmp;
    }

    SceneManager::SceneManager() {
        current = "NONE";
    }

    Scene* SceneManager::register_scene(const char* name, Scene* scene) {
        if (scenes.contains(name))
            assert("ERROR: Scene name already exists");
        scene->manager = this;
        scenes.insert(std::make_pair(name, scene));
        return scene;
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
}
