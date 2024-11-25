#ifndef EG_ASSERT

#define EG_ASSERT(condition, ...) \
    do { \
        if ((condition)) { \
            EG_CORE_CRITICAL(__VA_ARGS__); \
            std::abort(); \
        } \
    } while (0)

#endif
