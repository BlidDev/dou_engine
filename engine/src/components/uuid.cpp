#include "components/uuid.h"

#include <random>

namespace engine {
    static std::random_device random_device;
    static std::mt19937_64 generator(random_device());
    static std::uniform_int_distribution<uint64_t> uniform_distribution;

    UUID::UUID() {
        uuid = uniform_distribution(generator);
    }

    UUID::UUID(uint64_t uuid) {
        this->uuid = uuid;
    }

}
