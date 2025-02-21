#include "manager.h"
#include "systems.h"

namespace engine {
    SceneManager::SceneManager() {
        current = "NONE";
    }

    Scene* SceneManager::register_scene(const char* name, Scene* scene) {
        EG_ASSERT(scenes.contains(name), "Scene {} already exists", name);
        scene->manager = this;
        scenes.insert(std::make_pair(name, scene));
        return scene;
    }

    Scene* SceneManager::get_scene(const char* name) {
        Scene* ptr = nullptr;
        EG_ASSERT(scenes.find(name) == scenes.end(),"Scene {} does not exist", name);
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
        scene->uuids.clear();
    }

    Scene* SceneManager::get_current() {
        Scene* ptr = nullptr;
        EG_ASSERT(current == "NONE", "Current scene not set");
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


    void SceneManager::register_model(const char* name, VAOType format, float vertices[], unsigned int size) {
        EG_ASSERT(models.find(std::string(name)) != models.end(), "Model [{}] already registered", name);
        models.insert(std::make_pair(std::string(name), create_model(format, vertices, size)));
    }
}
