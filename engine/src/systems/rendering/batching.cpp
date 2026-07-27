#include "systems/rendering/batching.h"

namespace engine {
    // Layer (8 bits) | Shader (8 bits) | Texture (16 bits) | VAO (16 bits) | Misc (16 bits)
    uint64_t generate_sort_key(unsigned int layer, unsigned int vao, unsigned int shader, unsigned int texture, unsigned int misc) {
        return (static_cast<uint64_t>(layer   & 0xFF)   << 56) | // 8 bits  (0-255)
               (static_cast<uint64_t>(shader  & 0xFF)   << 48) | // 8 bits  (0-255)
               (static_cast<uint64_t>(texture & 0xFFFF) << 32) | // 16 bits (0-65535)
               (static_cast<uint64_t>(vao     & 0xFFFF) << 16) | // 16 bits (0-65535)
               (static_cast<uint64_t>(misc    & 0xFFFF));        // 16 bits (0-65535)
    }

}
