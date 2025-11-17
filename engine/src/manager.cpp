#include "manager.h"
#include "systems/physicsys.h"

namespace engine {
    SceneManager::SceneManager() {
        current = "NONE";
        project_data = ProjectData("Unnamed Project");
    }

    Scene* SceneManager::register_scene(const char* name, Scene* scene) {
        DU_ASSERT(scenes.contains(name), "Scene {} already exists", name);
        scene->manager = this;
        scenes.insert(std::make_pair(name, scene));
        DU_CORE_DEBUG_TRACE("Registered scene {}", name);
        return scene;
    }

    Scene* SceneManager::get_scene(const char* name) {
        Scene* ptr = nullptr;
        DU_ASSERT(scenes.find(name) == scenes.end(),"Scene {} does not exist", name);
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

    void SceneManager::clear_scene(Scene* scene) {
        end_actions(scene->registry);
        scene->registry.clear();
        scene->uuids.clear();
    }

    void SceneManager::end_scene(Scene* scene) {
        scene->on_end();
        end_actions(scene->registry);
        scene->registry.clear();
        scene->uuids.clear();
    }

    const std::filesystem::path& SceneManager::root_path() {
        return project_data.root_path;
    }

    Scene* SceneManager::get_current() {
        Scene* ptr = nullptr;
        DU_ASSERT(current == "NONE", "Current scene not set");
        DU_ASSERT(scenes.find(current) == scenes.end(), "Current set to non existant scene \"{}\"", current);
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

    namespace fs = std::filesystem;

    void SceneManager::register_shader(const char* path) {
        fs::path fs_path = fs::path(path);
        if (!fs_path.is_absolute()) fs_path = root_path() / fs_path;
        DU_ASSERT(shader_lib.find(fs_path.filename()) != shader_lib.end(), "Shader [{}] already registered", path);

        shader_lib.insert(std::make_pair(fs_path.filename().string(), complie_shader_file(fs_path.c_str())));

        DU_CORE_DEBUG_TRACE("Registered shader {}", fs_path.filename().string());
    }

    void SceneManager::register_texture(const char* path) {
        fs::path fs_path = fs::path(path);
        if (!fs_path.is_absolute()) fs_path = root_path() / fs_path;
        DU_ASSERT(texture_lib.find(fs_path.filename()) != texture_lib.end(), "Texture [{}] already registered", path);
        texture_lib.insert(std::make_pair(fs_path.filename().string(), load_texture_from_file(fs_path.c_str())));

        DU_CORE_DEBUG_TRACE("Registered texture {}", fs_path.filename().string());
    }

    void SceneManager::register_texture(std::string name, Texture texture) {
        DU_ASSERT(texture_lib.find(std::string(name)) != texture_lib.end(), "Texture [{}] already registered", name);

        texture_lib.insert(std::make_pair(name, texture));
        DU_CORE_DEBUG_TRACE("Registered texture {}", name);
    }

    void SceneManager::register_model(const char* name, Model model) {
        DU_ASSERT(model_lib.find(std::string(name)) != model_lib.end(), "Model [{}] already registered", name);
        model.name = name;
        model_lib.insert(std::make_pair(std::string(name), model));
        DU_CORE_DEBUG_TRACE("Registered model {}", name);
    }


    LayerAtrb* SceneManager::get_layer_atrb(size_t layer) {
        DU_ASSERT(layer >= MAX_RENDER_LAYERS || layer < 0, "Trying to retrieve invalid layer [{}]");
        return &render_data.layers_atrb[layer];
    }

    size_t SceneManager::num_of_scenes() {
        return scenes.size();
    }

    const std::unordered_map<std::string, Scene*>& SceneManager::get_scenes() {
        return scenes;
    }

    void update_render_data(SceneManager* manager, Scene* current) {
        manager->render_data.ambient = current->ambient;
        manager->render_data.ambient_strength = current->ambient_strength;
    }
}
