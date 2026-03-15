#pragma once
#include <filesystem>


namespace engine {

    struct Texture{ 
        Texture();

        uint32_t texture;
        std::string path;

        int w, h, nrc;

        operator uint32_t() {
            return texture;
        }


        // Frees the texture ID from OpenGL memory, makes the object useless
        void free();
    };

    Texture load_texture_from_file(const std::filesystem::path& path, bool flip = true);
}
