#include <espch.h>
#include <sys/stat.h>
#include <filesystem>
#include "project.h"
#include "manager.h"


#define TRY_NODE(node,name, type, lhs) if(node[name]){ lhs = node[name].as<type>();}

namespace engine {

    ProjectData::ProjectData(std::string name) {
        name = name;
        startup_scene = "";
        root_path = "";
        scene_paths = {};
        shader_paths = {};
        texture_paths = {};
        mesh_paths = {};
        script_paths = {};
    }

    static void read_layers(YAML::Node& node, SceneManager* manager);
    static void read_paths(YAML::Node& node, SceneManager* manager, bool add_paths);
    static void read_scenes(YAML::Node& node, SceneManager* manager, bool add_paths);

    const ProjectData& read_project_file(const char* path, SceneManager* manager, bool add_paths, bool set_current) {
        DU_ASSERT(!manager, "Trying to read project {} but SceneManager is null");

        std::ifstream file(path);
        DU_ASSERT(file.fail(), "Project file path \"{}\" does not exist", path);
        std::stringstream str_stream;
        str_stream<<file.rdbuf();

        YAML::Node data = YAML::Load(str_stream.str());
        ProjectData* p_data = &manager->project_data;

        DU_ASSERT(!data["Project Name"], "Project name not provided in \"{}\"", path);
        p_data->name = data["Project Name"].as<std::string>();

        std::string root_path = "";
        TRY_NODE(data, "Root Path", std::string, root_path);
        if(!root_path.empty())
            p_data->root_path = std::filesystem::path(root_path);


        ;
        TRY_NODE(data,"Startup Scene", std::string, p_data->startup_scene);

        read_layers(data, manager);
        read_paths(data, manager, add_paths);
        read_scenes(data, manager, add_paths);

        if(!set_current) return manager->project_data;
        DU_ASSERT(p_data->startup_scene.empty(),"set_current is true for project {} but \"Startup Scene\" was not provided", p_data->name);
        manager->set_current(p_data->startup_scene.c_str());
        return manager->project_data;
    }

    static void read_layers(YAML::Node& node, SceneManager* manager) {
        auto layers = node["Layers"];
        if (!layers) return;

        for(int i = 0; i < MAX_RENDER_LAYERS; i++) {
            std::string name = std::format("Layer {}", i);
            auto layer = layers[name];
            if (!layer) continue;
            TRY_NODE(layer, "Depth Test", bool, manager->get_layer_atrb(i)->depth);
            TRY_NODE(layer, "Wireframe Mode", bool, manager->get_layer_atrb(i)->wireframe);
        }
    }

    static void read_paths(YAML::Node& node, SceneManager* manager, bool add_paths) {
        namespace fs = std::filesystem; 
        ProjectData* p_data = &manager->project_data;
        auto paths = node["Paths"];
        if (!paths) return;

        auto shaders = paths["Shaders"];
        if (shaders) {
            for (const auto& path : shaders) {
                fs::path sh_dir = path.as<std::string>();
                fs::path actual = p_data->root_path / sh_dir;
                DU_ASSERT(!fs::is_directory(actual), "Trying to read shaders from {} no such directory", actual.string());
                if(add_paths) p_data->shader_paths.push_back(sh_dir);

                for (const auto & entry : fs::directory_iterator(actual)) {
                    if (entry.path().extension() != ".glsl") continue;
                    manager->register_shader((sh_dir / entry.path().filename()).c_str());
                }
            }
        }

        auto textures = paths["Textures"];
        if (textures) {
            auto flip_node = node["Flip Textures On Load"];
            bool flip = false;
            if (flip_node)
                flip = flip_node.as<bool>();

            for (const auto& path : textures) {
                fs::path tx_dir = path.as<std::string>();
                fs::path actual = p_data->root_path / tx_dir;
                DU_ASSERT(!fs::is_directory(actual), "Trying to read textures from {} no such directory", actual.string());
                if(add_paths) p_data->texture_paths.push_back(tx_dir);
                for (const auto & entry : fs::directory_iterator(actual)) {
                    fs::path tmp = entry.path();
                    std::array<const char*, 3> acceptables =  {".png", ".jpg", ".jpeg"};
                    bool contains = std::find(acceptables.begin(), acceptables.end(), tmp.extension()) != acceptables.end();
                    if (!contains) continue;
                    manager->register_texture((tx_dir / tmp.filename()).c_str(), flip);
                }
            }
        }

        auto meshs = paths["Meshes"];
        if (meshs) {
            for (const auto& path : meshs) {
                fs::path md_dir = path.as<std::string>();
                fs::path actual = p_data->root_path / md_dir;
                DU_ASSERT(!fs::is_directory(actual), "Trying to read meshs from {} no such directory", actual.string());
                if(add_paths) p_data->mesh_paths.push_back(md_dir);
                for (const auto & entry : fs::directory_iterator(actual)) {
                    if (entry.path().extension() != ".sff") continue;
                    std::string mesh_name = "unnamed";

                    Mesh mesh = mesh_from_file(entry.path().c_str(), &mesh_name);
                    DU_ASSERT(mesh_name == "unnamed", "Mesh {} wasn't given any name");
                    manager->register_mesh(mesh_name.c_str(), mesh);
                }

            }
        }

        auto scripts = paths["Scripts"];
        if (scripts) {
            for (const auto& path : scripts) {
                fs::path sh_dir = path.as<std::string>();
                fs::path actual = p_data->root_path / sh_dir;
                DU_ASSERT(!fs::is_directory(actual), "Trying to read scripts from {} no such directory", actual.string());
                if(add_paths) p_data->script_paths.push_back(sh_dir);

                for (const auto & entry : fs::directory_iterator(actual)) {
                    if (entry.path().extension() != ".lua") continue;
                    manager->register_script((sh_dir / entry.path().filename()).c_str());
                }
            }
        }

    }



    static void read_scenes(YAML::Node& node, SceneManager* manager, bool add_paths) {
        namespace fs = std::filesystem;
        auto paths = node["Paths"];
        if(!paths) return;
        auto scenes = paths["Scenes"];
        if(!scenes) return;

        for (const auto& path : scenes) {
            fs::path sc_dir = path.as<std::string>();
            fs::path actual = manager->project_data.root_path / sc_dir;
            DU_ASSERT(!fs::is_directory(actual), "Trying to read scenes from {} no such directory", actual.string());
            if(add_paths) manager->project_data.scene_paths.push_back(sc_dir);

            for (const auto& entry : fs::directory_iterator(actual)) {
                fs::path tmp = entry.path();
                if (tmp.extension() != ".scene") continue;
                std::string name = extract_scene_name(tmp.c_str());
                Scene* rt = manager->register_scene(name.c_str(), create_runtime_scene());
                rt->name = name;
                rt->file_path = tmp;
            }

        }

    }


    void write_project_file(const char* path, ProjectData& data, LayerAtrb layers[], size_t nlayers) {
        namespace ym = YAML;
        std::ofstream file(path);
        DU_ASSERT(!file.is_open(), "Could not open {}", path);
        DU_CORE_DEBUG_TRACE("Writing to {}", path);
        ym::Emitter out;
        out<<ym::BeginMap;
            out<<ym::Key<<"Project Name"<<ym::Value<<data.name;

            if(!data.root_path.empty()) 
                out<<ym::Key<<"Root Path"<<ym::Value<<data.root_path.string();
            
            if(!data.startup_scene.empty())
                out<<ym::Key<<"Startup Scene"<<ym::Value<<data.startup_scene;
            
            out<<ym::Key<<"Paths"<<ym::BeginMap;
                out<<ym::Key<<"Scenes"<<ym::BeginSeq;
                    for (const auto& p : data.scene_paths) { out<<ym::Key<<p.c_str(); }
                out<<ym::EndSeq;

                out<<ym::Key<<"Shaders"<<ym::BeginSeq;
                    for (const auto& p : data.shader_paths) { out<<ym::Key<<p.c_str(); }
                out<<ym::EndSeq;

                out<<ym::Key<<"Textures"<<ym::BeginSeq;
                    for (const auto& p : data.texture_paths) { out<<ym::Key<<p.c_str(); }
                out<<ym::EndSeq;

                out<<ym::Key<<"Meshes"<<ym::BeginSeq;
                    for (const auto& p : data.mesh_paths) { out<<ym::Key<<p.c_str(); }
                out<<ym::EndSeq;

                out<<ym::Key<<"Scripts"<<ym::BeginSeq;
                    for (const auto& p : data.script_paths) { out<<ym::Key<<p.c_str(); }
                out<<ym::EndSeq;
            out<<ym::EndMap;


            if(layers) {
                out<<ym::Key<<"Layers"<<ym::BeginMap;
                for(int i = 0; i < nlayers; i++) {
                    std::string name = std::format("Layer {}", i);
                    auto layer = layers[i];
                    out<<ym::Key<<name<<ym::BeginMap;
                        out<<ym::Key<<"Depth Test"<<ym::Value<<layer.depth;
                        out<<ym::Key<<"Wireframe Mode"<<ym::Value<<layer.wireframe;
                    out<<ym::EndMap;
                }

                out<<ym::EndMap;
            }

        out<<ym::EndMap;

        file<<out.c_str();
        file.close();
    }

}
