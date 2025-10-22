#pragma once
#include "log.h"

namespace engine {
    
    struct UUID {
        UUID();
        UUID(uint64_t uuid);
        UUID(const UUID&) = default;

        operator uint64_t() const {return uuid;}

        std::ostream& operator<<(std::ostream& stream) {
            stream<<uuid;
            return stream;
        }

        inline uint64_t get_uuid() { return uuid; }
        inline void display() { EG_CORE_INFO("{}", uuid); }
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

template<> 
struct fmt::formatter<engine::UUID> : fmt::formatter<std::string> {
    auto format(const engine::UUID& uuid, format_context& ctx) const {
        return fmt::formatter<std::string>::format(std::format("UUID({})", (uint64_t)uuid), ctx);
    }
};
