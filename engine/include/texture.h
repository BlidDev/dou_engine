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


        // Frees the texture ID from OpenGL memory, makes the object useless
        void free() {
            glDeleteTextures(1, &texture); 
            DU_CORE_DEBUG_TRACE("Freed {}", path);
            texture = 0;
            path = "UNKNOWN";
            w = -1; h = -1; nrc = -1;
        }
    };

    Texture load_texture_from_file(const char* path, bool flip = false);

}
