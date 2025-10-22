#pragma once
#include "log.h"

#define EG_ASSERT(condition, ...) \
    do { \
        if ((condition)) { \
            EG_CORE_CRITICAL(__VA_ARGS__); \
            std::abort(); \
        } \
    } while (0)

#define EG_HANDLEI(condition, res) \
    int tmp = condition; \
    if (tmp == res) {return res;}
