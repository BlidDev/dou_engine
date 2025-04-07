#pragma once
#include <espch.h>


namespace engine {

    struct Texture{ 
        Texture();

        uint32_t texture;
        std::string path;

        int w, h, nrc;

        operator uint32_t() {
            return texture;
        }
    };

    Texture load_texture_from_file(const char* path);

}
