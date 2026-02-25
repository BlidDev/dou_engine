#include "manager.h"
#include "components/camera.h"

namespace engine {
    SceneManager::SceneManager() : main_window() {
        current = "NONE";
        project_data = ProjectData("Unnamed Project");
        old = nullptr;
        switched = true;

        shader_lib = {};
        texture_lib = {};
        mesh_lib = {};
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
        auto cameras = scene->registry.view<CameraComp>();
        for (auto [e, c] : cameras.each())  {
            c.framebuffer.free(true);
        }
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

        for (auto& [_, s] : shader_lib) { s.free(); }
        for (auto& [_, t] : texture_lib) { t.free(); }
        for (auto& [_, m] : mesh_lib) { m.free(); }

    }

    SceneManager::~SceneManager() {
    }

    namespace fs = std::filesystem;

    void SceneManager::register_shader(const char* path) {
        fs::path fs_path = fs::path(path);
        if (!fs_path.is_absolute()) fs_path = root_path() / fs_path;

        DU_ASSERT(shader_lib.contains(fs_path.filename()), "Shader [{}] already registered", path);


        shader_lib.insert(std::make_pair(fs_path.filename().string(), complie_shader_file(fs_path.c_str())));
        DU_CORE_DEBUG_TRACE("Registered shader {}", fs_path.filename().string());
    }

    void SceneManager::register_shader(const char* name, const Shader& shader) {
        DU_ASSERT(shader_lib.contains(name), "Shader [{}] already registered", name);

        shader_lib.insert(std::make_pair(name, shader));
        DU_CORE_DEBUG_TRACE("Registered shader {}", name);
    }

    void SceneManager::register_texture(const char* path, bool flip) {
        fs::path fs_path = fs::path(path);
        if (!fs_path.is_absolute()) fs_path = root_path() / fs_path;


        DU_ASSERT(texture_lib.contains(fs_path.filename()), "Texture [{}] already registered", path);
        texture_lib.insert(std::make_pair(fs_path.filename().string(), load_texture_from_file(fs_path.c_str(), flip)));
        DU_CORE_DEBUG_TRACE("Registered texture {}", fs_path.filename().string());
    }

    void SceneManager::register_texture(std::string name, Texture texture) {
        DU_ASSERT(texture_lib.contains(name), "Texture [{}] already registered", name);

        texture_lib.insert(std::make_pair(name, texture));
        DU_CORE_DEBUG_TRACE("Registered texture {}", name);
    }

    void SceneManager::register_mesh(const char* name, Mesh mesh) {
        DU_ASSERT(mesh_lib.contains(name), "Mesh [{}] already registered", name);
        mesh.name = name;
        mesh_lib.insert(std::make_pair(std::string(name), mesh));
        DU_CORE_DEBUG_TRACE("Registered mesh {}", name);
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
}
