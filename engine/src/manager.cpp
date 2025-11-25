#include "manager.h"

namespace engine {
    SceneManager::SceneManager() : main_window() {
        current = "NONE";
        project_data = ProjectData("Unnamed Project");
        old = nullptr;
        switched = true;

        shader_lib = {};
        texture_lib = {};
        model_lib = {};
        script_lib = {};
    }

    Scene* SceneManager::register_scene(const char* name, std::unique_ptr<Scene> scene) {
        DU_ASSERT(scenes.contains(name), "Scene {} already exists", name);
        scene->manager = this;
        auto p = scenes.insert(std::make_pair(name, std::move(scene)));
        DU_CORE_DEBUG_TRACE("Registered scene {}", name);
        return p.first->second.get();
    }

    Scene* SceneManager::register_scene(const char* name, std::unique_ptr<Scene>& scene) {
        return register_scene(name, std::move(scene));
    }

    Scene* SceneManager::register_scene_raw(const char* name, Scene* scene) {
        register_scene(name, std::unique_ptr<Scene>(scene));
        return scene;
    }

    Scene* SceneManager::get_scene(const char* name) {
        const auto& it = scenes.find(name);
        DU_ASSERT(it == scenes.end(),"Scene {} does not exist", name);
        
        return it->second.get();
    }


    void SceneManager::set_current(const char* name) {
        switched = true;
        if (current != "NONE") {
            old = get_current();
        }
        if(get_scene(name))
            current = name;
    }

    void SceneManager::clear_scene(Scene* scene) {
        scene->registry.clear();
        scene->uuids.clear();
    }

    void SceneManager::end_scene(Scene* scene) {
        scene->on_end();
        scene->registry.clear();
        scene->uuids.clear();
    }

    const std::filesystem::path& SceneManager::root_path() {
        return project_data.root_path;
    }

    Scene* SceneManager::get_current() {
        DU_ASSERT(current == "NONE", "Current scene not set");

        const auto& it = scenes.find(current);
        DU_ASSERT(it == scenes.end(), "Current set to non existant scene \"{}\"", current);
        return it->second.get();
    }

    void SceneManager::end() {
        if ("NONE" != current)
            get_current()->on_end();
    }

    SceneManager::~SceneManager() {
        // automatically deletes every scene
    }

    namespace fs = std::filesystem;

    void SceneManager::register_shader(const char* path) {
        fs::path fs_path = fs::path(path);
        if (!fs_path.is_absolute()) fs_path = root_path() / fs_path;

        DU_ASSERT(shader_lib.contains(fs_path.filename()), "Shader [{}] already registered", path);


        shader_lib.insert(std::make_pair(fs_path.filename().string(), complie_shader_file(fs_path.c_str())));
        DU_CORE_DEBUG_TRACE("Registered shader {}", fs_path.filename().string());
    }

    void SceneManager::register_texture(const char* path) {
        fs::path fs_path = fs::path(path);
        if (!fs_path.is_absolute()) fs_path = root_path() / fs_path;


        DU_ASSERT(texture_lib.contains(fs_path.filename()), "Texture [{}] already registered", path);
        texture_lib.insert(std::make_pair(fs_path.filename().string(), load_texture_from_file(fs_path.c_str())));
        DU_CORE_DEBUG_TRACE("Registered texture {}", fs_path.filename().string());
    }

    void SceneManager::register_texture(std::string name, Texture texture) {
        DU_ASSERT(texture_lib.contains(name), "Texture [{}] already registered", name);

        texture_lib.insert(std::make_pair(name, texture));
        DU_CORE_DEBUG_TRACE("Registered texture {}", name);
    }

    void SceneManager::register_model(const char* name, Model model) {
        DU_ASSERT(model_lib.contains(name), "Model [{}] already registered", name);
        model.name = name;
        model_lib.insert(std::make_pair(std::string(name), model));
        DU_CORE_DEBUG_TRACE("Registered model {}", name);
    }

    void SceneManager::register_script(const char* path) {
        fs::path fs_path = fs::path(path);
        if (!fs_path.is_absolute()) fs_path = root_path() / fs_path;

        DU_ASSERT(script_lib.contains(fs_path.filename()), "Script path [{}] already registered", path);

        script_lib.insert({fs_path.filename().string(), fs_path.string()});
        DU_CORE_DEBUG_TRACE("Registered script {}", fs_path.filename().string());
    }

    LayerAtrb* SceneManager::get_layer_atrb(size_t layer) {
        DU_ASSERT(layer >= MAX_RENDER_LAYERS || layer < 0, "Trying to retrieve invalid layer [{}]");
        return &render_data.layers_atrb[layer];
    }

    size_t SceneManager::num_of_scenes() {
        return scenes.size();
    }

    const SceneLib& SceneManager::get_scenes() const {
        return scenes;
    }

    void update_render_data(SceneManager* manager, Scene* current) {
        manager->render_data.ambient = current->ambient;
        manager->render_data.ambient_strength = current->ambient_strength;
    }
}
