#pragma once
#include <cstdint>
#include <unordered_map>

namespace engine {
    
    struct UUID {
        UUID();
        UUID(uint64_t uuid);
        UUID(const UUID&) = default;

        operator uint64_t() const {return uuid;}
    private:
        uint64_t uuid;
    };
}

namespace std {

    template <>
    struct hash<engine::UUID> {
        std::size_t operator()(const engine::UUID& uuid) const { 
            return hash<uint64_t>()((uint64_t)uuid);
        }
    };

}
