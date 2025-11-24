#pragma once

// shaders
// textures
// current
// models
// layers

#include "renderer.h"

namespace engine {
    class SceneManager;
    class Scene;
    
    struct ProjectData {
        std::string name;
        std::string startup_scene;
        std::filesystem::path root_path;
        std::vector<std::filesystem::path> scene_paths;
        std::vector<std::filesystem::path> shader_paths;
        std::vector<std::filesystem::path> texture_paths;
        std::vector<std::filesystem::path> model_paths;

        ProjectData(std::string name = "");
    };

    const ProjectData& read_project_file(const char* path, SceneManager* manager, bool add_paths = true, bool set_current = false);


    void write_project_file(const char* path, ProjectData& data, LayerAtrb layers[] = nullptr, size_t nlayers = MAX_RENDER_LAYERS);


    extern std::unique_ptr<Scene> create_runtime_scene();
}
