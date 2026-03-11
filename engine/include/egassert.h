#pragma once
#include "log.h"

#define DU_ASSERT(condition, ...) \
    do { \
        if (condition) { \
            DU_CORE_CRITICAL(__VA_ARGS__); \
            std::abort(); \
        } \
    } while (0)

#define DU_THROW_ASSERT(message, ...) \
    DU_ASSERT(true, message, __VA_ARGS__)
