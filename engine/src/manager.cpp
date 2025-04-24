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


    void SceneManager::register_shader(const char* path) {
        EG_ASSERT(shader_lib.find(std::string(path)) != shader_lib.end(), "Shader [{}] already registered", path);
        shader_lib.insert(std::make_pair(std::string(path), complie_shader_file(path)));
    }

    void SceneManager::register_texture(const char* path) {
        EG_ASSERT(texture_lib.find(std::string(path)) != texture_lib.end(), "Texture [{}] already registered", path);
        texture_lib.insert(std::make_pair(std::string(path), load_texture_from_file(path)));
    }

    void SceneManager::register_model(const char* name, Model model) {
        EG_ASSERT(model_lib.find(std::string(name)) != model_lib.end(), "Model [{}] already registered", name);
        model.name = name;
        model_lib.insert(std::make_pair(std::string(name), model));
    }
}
