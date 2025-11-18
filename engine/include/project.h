#pragma once

// shaders
// textures
// current
// models
// layers

#include "renderer.h"
namespace fs = std::filesystem;


namespace engine {
    class SceneManager;
    class Scene;
    
    struct ProjectData {
        std::string name;
        std::string startup_scene;
        std::filesystem::path root_path;
        std::vector<fs::path> scene_paths;
        std::vector<fs::path> shader_paths;
        std::vector<fs::path> texture_paths;
        std::vector<fs::path> model_paths;

        ProjectData(std::string name = "");
    };

    const ProjectData& read_project_file(const char* path, SceneManager* manager, bool add_paths = true, bool set_current = false);


    void write_project_file(const char* path, ProjectData& data, LayerAtrb layers[] = nullptr, size_t nlayers = MAX_RENDER_LAYERS);


    extern Scene* create_runtime_scene();
}
