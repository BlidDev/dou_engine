#include "texture.h"

#define STBI_NO_SIMD
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
namespace engine {

    Texture::Texture() {
        texture = -1;
        path = "UNKNOWN";
        w = -1; h = -1; nrc = -1;
    }

    Texture load_texture_from_file(const char* path) {
        uint32_t texture;
        int w, h, nrc;

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        unsigned char *data = stbi_load(path, &w, &h, &nrc, 0);

        DU_ASSERT(!data, "Could not load texture [{}]", path);
        stbi_set_flip_vertically_on_load(true);

        auto format = (nrc == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);

        Texture res;
        res.texture = texture;
        res.path = std::filesystem::path(path).filename();
        res.w = w; res.h = h; res.nrc = nrc;

        //DU_INFO("{} {} {} {}", texture, w, h, nrc);

        return res;
    }

}
