#pragma once
#include "log.h"

#define DU_ASSERT(condition, ...) \
    do { \
        if ((condition)) { \
            DU_CORE_CRITICAL(__VA_ARGS__); \
            std::abort(); \
        } \
    } while (0)

